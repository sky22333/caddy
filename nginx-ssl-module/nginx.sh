#!/bin/bash

# 安装依赖
apt-get update
apt-get install -y build-essential libpcre3 libpcre3-dev zlib1g zlib1g-dev \
    libssl-dev libgd-dev libxml2 libxml2-dev uuid-dev libcurl4-openssl-dev \
    libjson-c-dev

# 下载并解压nginx源码
NGINX_VERSION="1.26.0"
wget "http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz"
tar -xzvf "nginx-${NGINX_VERSION}.tar.gz"
cd "nginx-${NGINX_VERSION}"

# 复制自动SSL模块到modules目录
mkdir -p auto/modules
cp ../auto_ssl_module.c auto/modules/

# 配置编译选项
./configure \
    --prefix=/usr/local/nginx \
    --with-http_ssl_module \
    --with-http_v2_module \
    --with-http_stub_status_module \
    --with-threads \
    --with-file-aio \
    --with-http_realip_module \
    --add-module=auto/modules/auto_ssl_module \
    --with-debug

# 编译并安装
make -j$(nproc)
make install

# 创建配置目录
mkdir -p /usr/local/nginx/conf/ssl/storage
chmod 700 /usr/local/nginx/conf/ssl
chmod 700 /usr/local/nginx/conf/ssl/storage

# 创建证书续期脚本
cat > /usr/local/nginx/sbin/renew-certs.sh <<EOF
#!/bin/bash
/usr/local/nginx/sbin/nginx -s reload
EOF

chmod +x /usr/local/nginx/sbin/renew-certs.sh

# 添加定时任务
(crontab -l 2>/dev/null; echo "0 0 * * * /usr/local/nginx/sbin/renew-certs.sh") | crontab -

# 基础nginx配置
cat > /usr/local/nginx/conf/nginx.conf <<EOF
worker_processes auto;
events {
    worker_connections 1024;
}

http {
    include mime.types;
    default_type application/octet-stream;
    
    # SSL配置
    ssl_session_timeout 1d;
    ssl_session_cache shared:SSL:50m;
    ssl_session_tickets off;
    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_ciphers ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES256-GCM-SHA384;
    ssl_prefer_server_ciphers off;
    
    # 多域名服务器配置示例
    server {
        listen 80;
        listen [::]:80;
        server_name example.com www.example.com blog.example.com;
        
        # 启用自动SSL
        auto_ssl on;
        auto_ssl_email admin@example.com;
        auto_ssl_domains example.com www.example.com blog.example.com;
        
        location / {
            root html;
            index index.html index.htm;
        }
    }
    
    # 可以添加更多server块支持其他域名
    server {
        listen 80;
        listen [::]:80;
        server_name another-domain.com www.another-domain.com;
        
        auto_ssl on;
        auto_ssl_email admin@another-domain.com;
        auto_ssl_domains another-domain.com www.another-domain.com;
        
        location / {
            root /var/www/another-domain;
            index index.html index.htm;
        }
    }
}
EOF

# 启动nginx
/usr/local/nginx/sbin/nginx
