// Zucks Server - Forwarder Module
// (c) 2026 Zucks Project. All rights reserved.

#include "zucks_forwarder.hpp"
#include "zucks_logger.hpp"

namespace Zucks {

Forwarder::Forwarder(asio::ip::tcp::socket client,
                     std::string target_host,
                     uint16_t target_port,
                     std::shared_ptr<Filter> filter)
    : client_(std::move(client)),
      resolver_(client_.get_executor()),
      server_(client_.get_executor()),
      target_host_(std::move(target_host)),
      target_port_(target_port),
      filter_(std::move(filter)) {}

void Forwarder::start() {
    asio::error_code ec;
    const auto ep = client_.remote_endpoint(ec);
    if (ec) {
        Logger::warn("Forwarder: cannot get remote endpoint: {}", ec.message());
        closeAll();
        return;
    }

    const std::string ip = ep.address().to_string();
    if (!filter_->allowConnection(ip)) {
        Logger::warn("Forwarder: connection blocked by filter for {}", ip);
        closeAll();
        return;
    }

    doResolve();
}

void Forwarder::doResolve() {
    auto self = shared_from_this();
    resolver_.async_resolve(
        target_host_,
        std::to_string(target_port_),
        [self](asio::error_code ec, asio::ip::tcp::resolver::results_type results) {
            if (ec) {
                Logger::warn("Forwarder: resolve failed: {}", ec.message());
                self->closeAll();
                return;
            }
            self->doConnect(std::move(results));
        });
}

void Forwarder::doConnect(asio::ip::tcp::resolver::results_type results) {
    auto self = shared_from_this();
    asio::async_connect(
        server_,
        results,
        [self](asio::error_code ec, const asio::ip::tcp::endpoint&) {
            if (ec) {
                Logger::warn("Forwarder: connect failed: {}", ec.message());
                self->closeAll();
                return;
            }
            self->startRelay();
        });
}

void Forwarder::startRelay() {
    Logger::info("Forwarder: relay started -> {}:{}", target_host_, target_port_);
    relayClientToServer();
    relayServerToClient();
}

void Forwarder::relayClientToServer() {
    auto self = shared_from_this();
    client_.async_read_some(
        asio::buffer(c2s_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) {
                self->closeAll();
                return;
            }

            if (!self->filter_->inspectPacket(
                    reinterpret_cast<const uint8_t*>(self->c2s_.data()), n)) {
                Logger::warn("Forwarder: packet blocked by DPI/filter");
                self->closeAll();
                return;
            }

            asio::async_write(
                self->server_,
                asio::buffer(self->c2s_, n),
                [self](asio::error_code ec2, std::size_t) {
                    if (ec2) {
                        self->closeAll();
                        return;
                    }
                    self->relayClientToServer();
                });
        });
}

void Forwarder::relayServerToClient() {
    auto self = shared_from_this();
    server_.async_read_some(
        asio::buffer(s2c_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) {
                self->closeAll();
                return;
            }

            asio::async_write(
                self->client_,
                asio::buffer(self->s2c_, n),
                [self](asio::error_code ec2, std::size_t) {
                    if (ec2) {
                        self->closeAll();
                        return;
                    }
                    self->relayServerToClient();
                });
        });
}

void Forwarder::closeAll() {
    asio::error_code ec;
    client_.close(ec);
    server_.close(ec);
}

} // namespace Zucks