// Zucks Server - Forwarder Module
// (c) 2026 Zucks Project. All rights reserved.

#pragma once

#include <asio.hpp>
#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "zucks_filter.hpp"

namespace Zucks {

class Forwarder : public std::enable_shared_from_this<Forwarder> {
public:
    Forwarder(asio::ip::tcp::socket client,
              std::string target_host,
              uint16_t target_port,
              std::shared_ptr<Filter> filter);

    void start();

private:
    void doResolve();
    void doConnect(asio::ip::tcp::resolver::results_type results);
    void startRelay();
    void relayClientToServer();
    void relayServerToClient();
    void closeAll();

    asio::ip::tcp::socket client_;
    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket server_;
    std::string target_host_;
    uint16_t target_port_;
    std::shared_ptr<Filter> filter_;

    std::array<char, 8192> c2s_{};
    std::array<char, 8192> s2c_{};
};

} // namespace Zucks