example.com {
    reverse_proxy http://no.example.net:8080 {
        header_up Host {host}
        header_up X-Real-IP {remote}
        header_up X-Forwarded-For {remote}
    }

    # 使用 DNS-over-HTTPS 配置 Google DNS
    resolvers https://dns.google/dns-query
}
