### Angie是一个高效、强大且可扩展的网络服务器、从 nginx 分叉而来。

由nginx原项目的前开发者创建，以实现新的发展方向，并作为nginx替代品，无需重写配置或模块。包含了`nginx` 1.27.4 的所有功能，并增加了许多新特性。例如像`caddy`那种自动配置证书等等。

官方文档：https://cn.angie.software/angie/docs/

官方仓库：https://github.com/webserver-llc/angie

相关docker镜像(非官方)：https://hub.docker.com/r/eilandert/angie/tags

### 一键安装

```
sudo sh -c 'curl -fsSL https://angie.software/keys/angie-signing.gpg -o /etc/apt/trusted.gpg.d/angie.gpg && echo "deb https://download.angie.software/angie/$(. /etc/os-release; echo $ID/$VERSION_ID $VERSION_CODENAME) main" > /etc/apt/sources.list.d/angie.list && apt update && apt install -y angie'
```

### 或者分步安装
```
# 添加 Angie 官方 GPG 密钥
sudo curl -fsSL https://angie.software/keys/angie-signing.gpg -o /etc/apt/trusted.gpg.d/angie.gpg

# 添加 Angie 软件源
echo "deb https://download.angie.software/angie/$(. /etc/os-release; echo $ID/$VERSION_ID $VERSION_CODENAME) main" | sudo tee /etc/apt/sources.list.d/angie.list

# 更新 apt 包索引
sudo apt update

# 安装 Angie
sudo apt install -y angie
```


### 常用命令
```
# 检查配置文件语法
sudo angie -t

# 重启服务
sudo systemctl restart angie

# 停止服务
sudo systemctl stop angie

# 重新加载配置（不中断连接）
sudo systemctl reload angie

# 查看服务状态
sudo systemctl status angie

# 设置开机自启
sudo systemctl enable angie

# 查看系统日志
sudo journalctl -u angie -f
```


### 配置

默认配置路径：`/etc/angie/angie.conf`，跟nginx配置语法大致相同。


### 配置文件结构
默认目录结构
```
/etc/angie/
├── angie.conf              # 主配置文件
├── mime.types              # MIME类型定义
├── fastcgi_params          # FastCGI参数
├── scgi_params             # SCGI参数
├── uwsgi_params            # uWSGI参数
├── conf.d/                 # 额外配置目录
│   └── default.conf        # 默认虚拟主机配置
├── sites-available/        # 可用站点配置
│   └── default             # 默认站点配置
├── sites-enabled/          # 启用站点配置（符号链接）
│   └── default -> ../sites-available/default
└── snippets/               # 配置片段
    ├── fastcgi-php.conf
    └── snakeoil.conf
```

### SSL证书目录（ACME自动获取）
```
/var/lib/angie/acme/             # 证书存储目录
└── letsencrypt/
    ├── cert.pem
    ├── private.key
    └── chain.pem
```

### 完全卸载
```
# 1. 停止 Angie 服务
sudo systemctl stop angie

# 2. 禁用 Angie 服务，避免开机自启
sudo systemctl disable angie

# 3. 卸载 Angie 软件包
sudo apt remove --purge -y angie

# 4. 删除 Angie 软件源文件
sudo rm -f /etc/apt/sources.list.d/angie.list

# 5. 删除 Angie 官方 GPG 密钥文件
sudo rm -f /etc/apt/trusted.gpg.d/angie.gpg

# 6. 更新 apt 包索引（移除源后更新）
sudo apt update

# 7. 删除 Angie 相关的日志和配置目录
sudo rm -rf /var/log/angie
sudo rm -rf /etc/angie
sudo rm -rf /run/angie.pid

# 8. 查看是否有残留
dpkg -l | grep angie
```
