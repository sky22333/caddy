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




### 2：效果演示

 ![alt](/static/home.jpg)

---

#### 声明：我只是个搬运工，原作者[在这里](https://github.com/ZYYO666)
