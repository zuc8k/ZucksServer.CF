#pragma once

#include <asio.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include "zucks_filter.hpp"

namespace Zucks {

class Proxy : public std::enable_shared_from_this<Proxy> {
public:
    Proxy(asio::io_context& io,
          uint16_t listen_port,
          std::string target_host,
          uint16_t target_port,
          std::shared_ptr<Filter> filter);

    void start();
    void stop();

private:
    void doAccept();

    asio::io_context& io_;
    asio::ip::tcp::acceptor acceptor_;
    std::string target_host_;
    uint16_t target_port_;
    std::shared_ptr<Filter> filter_;
};

} // namespace Zucks