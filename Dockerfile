FROM golang:1.21-alpine AS builder

RUN go install github.com/caddyserver/xcaddy/cmd/xcaddy@latest

RUN ls -l /go/bin/

# 使用 xcaddy 构建 Caddy 并安装插件
RUN xcaddy build \
    --with github.com/caddyserver/caddy/v2 \
    --with github.com/mholt/caddy-ratelimit \
    --with github.com/lanrat/caddy-dynamic-remoteip \
    --with github.com/greenpau/caddy-security

RUN ls -l /go/bin/

FROM alpine

RUN apk add --no-cache libc6-compat

COPY --from=builder /go/bin/caddy /usr/bin/caddy

RUN ls -l /usr/bin/caddy

WORKDIR /app

COPY Caddyfile /etc/caddy/Caddyfile

EXPOSE 80 443

CMD ["caddy", "run", "--config", "/etc/caddy/Caddyfile"]
