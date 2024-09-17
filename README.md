# Nginx文档


<details>
  <summary>nginx常用命令</summary>

| 命令                          | 说明                     |
|-------------------------------|--------------------------|
| `sudo apt update && sudo apt install -yq nginx` | 安装 Nginx               |
| `sudo systemctl start nginx`   | 启动 Nginx               |
| `sudo systemctl stop nginx`    | 停止 Nginx               |
| `sudo systemctl restart nginx` | 重启 Nginx               |
| `sudo nginx -t`                | 检查 Nginx 配置           |
| `sudo journalctl -u nginx`                | 查看 Nginx 日志           |
| `sudo systemctl reload nginx`  | 重新加载 Nginx 配置      |
| `sudo systemctl status nginx`  | 查看 Nginx 状态          |
| `sudo systemctl enable nginx`  | 启用 Nginx 开机自启动    |
| `sudo systemctl disable nginx` | 禁用 Nginx 开机自启动    |
| `sudo apt purge nginx` | 完全卸载 Ngxin          |

| 配置文件路径                  | 说明                     |
|-------------------------------|--------------------------|
| `/etc/nginx/nginx.conf`        | 主配置文件               |
| `/etc/nginx/sites-available/`  | 站点配置文件目录         |
| `/etc/nginx/sites-enabled/`    | 启用的站点配置文件目录   |
| `tail -f /var/log/nginx/access.log`    | 查看访问日志             |
| `tail -f /var/log/nginx/error.log`     | 查看错误日志             |
| `/etc/nginx/conf.d/`           | 自定义配置文件目录       |
| [Nginx 配置生成器](https://www.digitalocean.com/community/tools/nginx) | 在线生成 Nginx 配置 |

    
</details>



---

# Caddy文档

<details>
  <summary>Docker部署</summary>

`docker-compose.yaml`配置

```
services:
  caddy:
    image: caddy:alpine
    container_name: caddy
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - /etc/caddy/Caddyfile:/etc/caddy/Caddyfile    # 映射配置文件
      - /var/www/html:/srv                           # 映射站点目录
    restart: always
```
    
</details>

---

<details>
  <summary>apt安装caddy命令</summary>


```
sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https curl
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo gpg --dearmor -o /usr/share/keyrings/caddy-stable-archive-keyring.gpg
curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list
sudo apt update
sudo apt install caddy -yq
```

</details>



#### 一键安装：
```
sudo apt install -yq debian-keyring debian-archive-keyring apt-transport-https curl && curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo gpg --dearmor -o /usr/share/keyrings/caddy-stable-archive-keyring.gpg && curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list && sudo apt update -q && sudo apt install -yq caddy
```

| 命令或路径                                          | 说明                         |
|-----------------------------------------------------|------------------------------|
| `sudo systemctl restart caddy`                      | 启动或重启 Caddy                   |
| `sudo systemctl status caddy`                            | 查看 Caddy 运行状态           |
| `sudo journalctl -u caddy`                              | 查看 Caddy 日志           |
| `caddy validate --config /etc/caddy/Caddyfile`      | 检查 Caddy 配置文件           |
| `sudo systemctl stop caddy`                         | 停止 Caddy 服务               |
| `sudo apt-get purge --auto-remove caddy`            | 完全卸载 Caddy                |
| `/etc/caddy/Caddyfile`                              | Caddy 配置文件路径            |


Caddy 证书存储路径  `/var/lib/caddy/.local/share/caddy/certificates/acme-v02.api.letsencrypt.org-directory`


#### 反向代理
```
example.com {
    encode gzip
    reverse_proxy localhost:8080
}
```

#### 代理站点目录
```
example.com {
    root * /var/www
    encode zstd gzip
    file_server
}
```


---

