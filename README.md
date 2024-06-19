### 1：介绍

一个静态导航页

一个静态发布页


# caddy教程：

#### Docker(可选)
```
docker run -d --name caddy -v /etc/caddy/Caddyfile:/etc/caddy/Caddyfile -v /var/wwww/home:/srv -p 80:80 -p 443:443 caddy
```


#### 安装：
```
sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https curl
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo gpg --dearmor -o /usr/share/keyrings/caddy-stable-archive-keyring.gpg
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list
sudo apt update
sudo apt install caddy
```




#### 一键安装：
```
sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https curl && curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo gpg --dearmor -o /usr/share/keyrings/caddy-stable-archive-keyring.gpg && curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list && sudo apt update && sudo apt install -y caddy
```

#### 启动：
```
sudo systemctl restart caddy
```

#### 运行状态
```
systemctl status caddy
```

#### 停止服务
```
sudo systemctl stop caddy
```

#### 完全卸载
```
sudo apt-get purge --auto-remove caddy
```

#### 配置文件：

路径`/etc/caddy/Caddyfile`

内容

```
yourdomain.com {
    root * /var/www/web
    file_server browse
    encode zstd gzip
    try_files {path} /index.html

    header {
        # 启用HSTS，提高安全性
        Strict-Transport-Security "max-age=31536000; includeSubDomains; preload"
        Cache-Control "public, max-age=31536000"
        X-XSS-Protection "1; mode=block"
        X-Content-Type-Options "nosniff"
    }

    # 缓存
    @cache {
        path_regexp cache .*\.(css|js|png|jpg|jpeg|gif|svg|woff|woff2|eot|ttf|otf|ico|webp)$
    }
    header @cache Cache-Control "public, max-age=31536000, immutable"
}
```
#### 证书路径
```
/var/lib/caddy/.local/share/caddy/certificates/acme-v02.api.letsencrypt.org-directory
```

#### 反代配置
```
yourdomain.com {
    reverse_proxy localhost:8333
}
```

#### 面板伪装
```
yourdomain.com {
    # 访问域名重定向到Bing
    redir / https://www.bing.com{uri}

    # 访问特定路径转发到面板
    reverse_proxy /admin/* localhost:1234
}
```
`面板需监听本地`


---

