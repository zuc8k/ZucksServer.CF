#include "zucks_proxy.hpp"
#include "zucks_logger.hpp"
#include <array>

namespace Zucks {
namespace {

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(asio::ip::tcp::socket client, std::string host, uint16_t port)
        : client_(std::move(client)),
          resolver_(client_.get_executor()),
          server_(client_.get_executor()),
          host_(std::move(host)),
          port_(port) {}

    void start() { doResolve(); }

private:
    void doResolve() {
        auto self = shared_from_this();
        resolver_.async_resolve(host_, std::to_string(port_),
            [self](auto ec, auto results) {
                if (!ec) self->doConnect(std::move(results));
            });
    }

    void doConnect(asio::ip::tcp::resolver::results_type results) {
        auto self = shared_from_this();
        asio::async_connect(server_, results,
            [self](auto ec, auto) {
                if (!ec) self->startRelay();
            });
    }

    void startRelay() {
        relayClientToServer();
        relayServerToClient();
    }

    void relayClientToServer() {
        auto self = shared_from_this();
        client_.async_read_some(asio::buffer(c2s_),
            [self](auto ec, auto n) {
                if (!ec) {
                    asio::async_write(self->server_, asio::buffer(self->c2s_, n),
                        [self](auto ec2, auto) {
                            if (!ec2) self->relayClientToServer();
                        });
                }
            });
    }

    void relayServerToClient() {
        auto self = shared_from_this();
        server_.async_read_some(asio::buffer(s2c_),
            [self](auto ec, auto n) {
                if (!ec) {
                    asio::async_write(self->client_, asio::buffer(self->s2c_, n),
                        [self](auto ec2, auto) {
                            if (!ec2) self->relayServerToClient();
                        });
                }
            });
    }

    asio::ip::tcp::socket client_;
    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket server_;
    std::string host_;
    uint16_t port_;
    std::array<char, 8192> c2s_{};
    std::array<char, 8192> s2c_{};
};

} // namespace

Proxy::Proxy(asio::io_context& io,
             uint16_t listen_port,
             std::string target_host,
             uint16_t target_port)
    : io_(io),
      acceptor_(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), listen_port)),
      target_host_(std::move(target_host)),
      target_port_(target_port) {}

void Proxy::start() { doAccept(); }

void Proxy::stop() {
    asio::error_code ec;
    acceptor_.close(ec);
}

void Proxy::doAccept() {
    auto self = shared_from_this();
    acceptor_.async_accept(
        [self](auto ec, auto socket) {
            if (!ec) {
                std::make_shared<Session>(
                    std::move(socket), self->target_host_, self->target_port_
                )->start();
            }
            if (self->acceptor_.is_open()) self->doAccept();
        });
}

} // namespace Zucks