// auto_ssl_module.c
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_event.h>
#include <openssl/ssl.h>
#include <curl/curl.h>
#include <json-c/json.h>

// 域名配置结构
typedef struct {
    ngx_str_t   domain;
    time_t      cert_expire;
    ngx_flag_t  is_valid;
} domain_cert_t;

// 证书存储结构
typedef struct {
    ngx_array_t  *domains;    // array of domain_cert_t
    ngx_str_t     storage_path;
    ngx_msec_t    check_interval;
} cert_storage_t;

// 模块配置结构
typedef struct {
    ngx_flag_t     enable;
    ngx_str_t      email;
    ngx_array_t   *domains;   // array of ngx_str_t
    ngx_str_t      cert_path;
    ngx_str_t      key_path;
    cert_storage_t *cert_storage;
    ngx_msec_t     renewal_threshold; // 续期阈值，默认30天
} ngx_http_auto_ssl_loc_conf_t;

// 全局变量
static cert_storage_t *g_cert_storage;
static ngx_event_t cert_check_event;

// 函数声明
static void check_cert_renewal(ngx_event_t *ev);
static ngx_int_t init_cert_storage(ngx_cycle_t *cycle);
static ngx_int_t renew_cert(domain_cert_t *domain);
static void schedule_cert_check(ngx_msec_t delay);

// 证书存储初始化
static ngx_int_t init_cert_storage(ngx_cycle_t *cycle) {
    g_cert_storage = ngx_pcalloc(cycle->pool, sizeof(cert_storage_t));
    if (g_cert_storage == NULL) {
        return NGX_ERROR;
    }

    g_cert_storage->domains = ngx_array_create(cycle->pool, 4, sizeof(domain_cert_t));
    if (g_cert_storage->domains == NULL) {
        return NGX_ERROR;
    }

    g_cert_storage->check_interval = 24 * 60 * 60 * 1000; // 24小时检查一次
    g_cert_storage->storage_path = (ngx_str_t)ngx_string("/usr/local/nginx/conf/ssl/storage");

    // 创建存储目录
    if (ngx_create_dir((char *)g_cert_storage->storage_path.data, 0700) == NGX_ERROR) {
        if (errno != EEXIST) {
            ngx_log_error(NGX_LOG_ERR, cycle->log, 0, "Failed to create cert storage directory");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}

// 证书检查和续期函数
static void check_cert_renewal(ngx_event_t *ev) {
    time_t now = ngx_time();
    domain_cert_t *domain;
    ngx_uint_t i;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ev->log, 0, "Checking certificates for renewal");

    if (g_cert_storage == NULL || g_cert_storage->domains == NULL) {
        goto done;
    }

    domain = g_cert_storage->domains->elts;
    for (i = 0; i < g_cert_storage->domains->nelts; i++) {
        // 检查证书是否需要续期（30天内过期）
        if (domain[i].cert_expire - now < 30 * 24 * 60 * 60) {
            if (renew_cert(&domain[i]) != NGX_OK) {
                ngx_log_error(NGX_LOG_ERR, ev->log, 0,
                    "Failed to renew certificate for domain: %V", &domain[i].domain);
            }
        }
    }

done:
    // 安排下一次检查
    schedule_cert_check(g_cert_storage->check_interval);
}

// 证书续期函数
static ngx_int_t renew_cert(domain_cert_t *domain) {
    // 实现证书续期逻辑
    // 1. 验证域名所有权
    // 2. 请求新证书
    // 3. 安装新证书
    // 4. 更新过期时间
    
    return NGX_OK;
}

// 安排下一次证书检查
static void schedule_cert_check(ngx_msec_t delay) {
    cert_check_event.handler = check_cert_renewal;
    cert_check_event.log = ngx_cycle->log;
    cert_check_event.data = NULL;
    
    ngx_add_timer(&cert_check_event, delay);
}

// 添加域名到存储
static ngx_int_t add_domain_to_storage(ngx_str_t *domain) {
    domain_cert_t *cert;
    
    if (g_cert_storage == NULL || g_cert_storage->domains == NULL) {
        return NGX_ERROR;
    }
    
    cert = ngx_array_push(g_cert_storage->domains);
    if (cert == NULL) {
        return NGX_ERROR;
    }
    
    cert->domain = *domain;
    cert->is_valid = 0;
    cert->cert_expire = 0;
    
    return NGX_OK;
}

// 进程初始化函数
static ngx_int_t ngx_http_auto_ssl_init_process(ngx_cycle_t *cycle) {
    if (init_cert_storage(cycle) != NGX_OK) {
        return NGX_ERROR;
    }
    
    // 立即开始第一次证书检查
    schedule_cert_check(1000);
    
    return NGX_OK;
}

// 主处理函数更新
static ngx_int_t ngx_http_auto_ssl_handler(ngx_http_request_t *r) {
    ngx_http_auto_ssl_loc_conf_t *aslcf;
    ngx_str_t *domains;
    ngx_uint_t i;
    
    aslcf = ngx_http_get_module_loc_conf(r, ngx_http_auto_ssl_module);
    if (!aslcf->enable) {
        return NGX_DECLINED;
    }

    // 检查所有配置的域名
    domains = aslcf->domains->elts;
    for (i = 0; i < aslcf->domains->nelts; i++) {
        if (ngx_strcasecmp(domains[i].data, r->headers_in.server.data) == 0) {
            // 找到匹配的域名，检查证书状态
            domain_cert_t *domain_certs = g_cert_storage->domains->elts;
            ngx_uint_t j;
            
            for (j = 0; j < g_cert_storage->domains->nelts; j++) {
                if (ngx_strcasecmp(domain_certs[j].domain.data, domains[i].data) == 0) {
                    if (!domain_certs[j].is_valid) {
                        // 需要获取或更新证书
                        if (obtain_ssl_cert(aslcf, &domains[i]) != NGX_OK) {
                            return NGX_HTTP_INTERNAL_SERVER_ERROR;
                        }
                    }
                    break;
                }
            }
        }
    }

    // HTTP到HTTPS重定向逻辑保持不变
    if (!r->connection->ssl) {
        ngx_table_elt_t *location;
        ngx_str_t new_uri;
        
        location = ngx_list_push(&r->headers_out.headers);
        if (location == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        
        new_uri.len = sizeof("https://") - 1 + r->headers_in.server.len + r->uri.len;
        new_uri.data = ngx_pnalloc(r->pool, new_uri.len);
        if (new_uri.data == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        
        ngx_sprintf(new_uri.data, "https://%V%V", &r->headers_in.server, &r->uri);
        
        location->hash = 1;
        location->key.len = sizeof("Location") - 1;
        location->key.data = (u_char *) "Location";
        location->value = new_uri;
        
        return NGX_HTTP_MOVED_PERMANENTLY;
    }

    return NGX_DECLINED;
}
