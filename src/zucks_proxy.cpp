#include "zucks_proxy.hpp"
#include "zucks_forwarder.hpp"
#include "zucks_logger.hpp"

namespace Zucks {

Proxy::Proxy(asio::io_context& io,
             uint16_t listen_port,
             std::string target_host,
             uint16_t target_port,
             std::shared_ptr<Filter> filter)
    : io_(io),
      acceptor_(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), listen_port)),
      target_host_(std::move(target_host)),
      target_port_(target_port),
      filter_(std::move(filter)) {}

void Proxy::start() {
    doAccept();
}

void Proxy::stop() {
    asio::error_code ec;
    acceptor_.close(ec);
}

void Proxy::doAccept() {
    auto self = shared_from_this();
    acceptor_.async_accept(
        [self](asio::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<Forwarder>(
                    std::move(socket),
                    self->target_host_,
                    self->target_port_,
                    self->filter_)
                    ->start();
            } else {
                Logger::warn("Proxy: accept failed: {}", ec.message());
            }

            if (self->acceptor_.is_open()) {
                self->doAccept();
            }
        });
}

} // namespace Zucks