FROM golang:1.21-alpine AS builder

RUN go install github.com/caddyserver/xcaddy/cmd/xcaddy@latest

# 使用 xcaddy 构建 Caddy 并安装插件
RUN xcaddy build \
    --with github.com/caddyserver/caddy/v2 \
    --with github.com/mholt/caddy-ratelimit \
    --with github.com/lanrat/caddy-dynamic-remoteip \
    --with github.com/greenpau/caddy-security \
    --output /app/caddy

FROM alpine

RUN apk add --no-cache libc6-compat

COPY --from=builder /app/caddy /usr/bin/caddy

WORKDIR /app

RUN touch /etc/caddy/Caddyfile

EXPOSE 80 443

CMD ["caddy", "run", "--config", "/etc/caddy/Caddyfile"]
