### 1：介绍

三个主题:白色-黑色-蓝色渐变

加了数百个精美动画-页面效果，页面交互，可操纵性已然达到巅峰

原生js，未使用任何插件

原生css

纯静态前端页面

小白教程：只需安装`nginx`或者`caddy`然后把下载的[源码](https://github.com/taotao1058/home/releases)解压放入网站根目录即可

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
    # 当访问根域名时，重定向到Bing
    redir / https://www.bing.com{uri}

    # 当访问特定路径（例如 /admin）时，转发到特定服务
    reverse_proxy /admin/* localhost:1234
}
```
#### xray端口复用
```
yourdomain.com {
    # 当访问根域名时，重定向到 Bing
    redir / https://www.bing.com{uri}

    # 当访问 /admin/* 路径时，转发到面板
    reverse_proxy /admin/* localhost:1234

    # 当访问 /vmess/* 路径时，流量通过 xray 的 vmess+ws 节点
    reverse_proxy /vmess/* localhost:12345 {
        # WebSocket
        header_up Upgrade {http.request.header.Upgrade}
        header_up Connection {http.request.header.Connection}
    }
}
```

`客户端节点端口需改为443`

### 2：效果演示

 ![alt](/static/home.jpg)

---

#### 声明：我只是个搬运工，原作者[在这里](https://github.com/ZYYO666)
