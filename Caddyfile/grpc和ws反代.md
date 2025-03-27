example.com {
    # gRPC 服务（HTTP/2）
    reverse_proxy /grpc/* h2c://127.0.0.1:8888
    # ws或者wss服务
    reverse_proxy /ws/* 127.0.0.1:9999
}
