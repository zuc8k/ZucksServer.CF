Proxy::Proxy(asio::io_context& io, const Config& cfg, std::shared_ptr<Filter> filter)
    : io_(io), acceptor_(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), cfg.listen_port)), cfg_(cfg), filter_(filter) {}

void Proxy::doAccept() {
    auto self = shared_from_this();
    acceptor_.async_accept([self](asio::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
            std::make_shared<Forwarder>(std::move(socket), self->cfg_, self->filter_)->start();
        }
        if (self->acceptor_.is_open()) self->doAccept();
    });
}