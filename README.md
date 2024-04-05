### 1：介绍

一个静态导航页

一个静态发布页


# caddy配置教程：

#### 安装：
```
sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https curl
```
```
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo gpg --dearmor -o /usr/share/keyrings/caddy-stable-archive-keyring.gpg
```
```
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list
```
```
sudo apt update
```
```
sudo apt install caddy
```

#### 配置文件：

路径`/etc/caddy/Caddyfile`

内容

```
yourdomain.com {
    root * /var/www/yourdomain
    file_server
    encode gzip
}

http://yourdomain.com {
    redir https://yourdomain.com{uri}
}
```

#### 启动：
```
sudo systemctl restart caddy
```

#### 多个端口反代多个站点
```
:2015 {
    root * /var/www/home1
    file_server
    encode gzip
}

:2016 {
    root * /var/www/home2
    file_server
    encode gzip
}

:2017 {
    root * /var/www/home3
    file_server
    encode gzip
}
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

