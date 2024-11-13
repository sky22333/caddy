FROM golang:1.21-alpine AS builder

RUN go install github.com/caddyserver/xcaddy/cmd/xcaddy@latest

# 构建Caddy并安装插件
RUN xcaddy build \
    --with github.com/caddyserver/caddy/v2 \
    --with github.com/mholt/caddy-ratelimit \
    --with github.com/lanrat/caddy-dynamic-remoteip \
    --with github.com/greenpau/caddy-security

FROM alpine

COPY --from=builder /go/bin/caddy /usr/bin/caddy

WORKDIR /app

COPY Caddyfile /etc/caddy/Caddyfile

EXPOSE 80 443

CMD ["caddy", "run", "--config", "/etc/caddy/Caddyfile"]
