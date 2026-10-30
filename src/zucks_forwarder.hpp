#pragma once
#include <asio.hpp>
#include <array>
#include <memory>
#include <string>
#include "zucks_filter.hpp"
#include "zucks_config.hpp"

namespace Zucks {

class Forwarder : public std::enable_shared_from_this<Forwarder> {
public:
    Forwarder(asio::ip::tcp::socket client, const Config& cfg, std::shared_ptr<Filter> filter);
    void start();

private:
    void readFirstPacket();
    void connectToTarget(const std::string& detected_version);
    void startRelay();
    void relayClientToServer();
    void relayServerToClient();
    void closeAll();

    asio::ip::tcp::socket client_;
    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket server_;
    const Config& cfg_;
    std::shared_ptr<Filter> filter_;

    std::array<char, 8192> c2s_{};
    std::array<char, 8192> s2c_{};
    
    TargetConfig current_target_; // الهدف الذي تم اختياره بناءً على النسخة
};

} // namespace Zucks