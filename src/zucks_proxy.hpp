// Zucks Server - Proxy Module
// (c) 2026 Zucks Project. All rights reserved.

#pragma once
#include <asio.hpp>
#include <memory>

namespace Zucks {

class Proxy : public std::enable_shared_from_this<Proxy> {
public:
    Proxy(asio::io_context& io, const std::string& listen_port, const std::string& target_host, const std::string& target_port);
    void start();

private:
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket client_socket_;
    asio::ip::tcp::socket server_socket_;
    std::string target_host_, target_port_;
    void handleClient(asio::ip::tcp::socket client);
};

} // namespace Zucks