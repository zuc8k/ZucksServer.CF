#include "zucks_forwarder.hpp"
#include "zucks_protocol_detector.hpp"
#include "zucks_logger.hpp"

namespace Zucks {

// ... (نفس الكونستركتور) ...

void Forwarder::readFirstPacket() {
    auto self = shared_from_this();
    client_.async_read_some(asio::buffer(c2s_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) { self->closeAll(); return; }

            // 1. اكتشاف الإصدار
            std::string version = ProtocolDetector::detect(
                reinterpret_cast<const uint8_t*>(self->c2s_.data()), n, self->cfg_.headers_map);

            Logger::info("Forwarder: Client connected. Header matched version: {}", version);

            // 2. تحديد السيرفر الهدف (لو الإصدار مجهول، نستخدم الافتراضي)
            if (self->cfg_.versions.find(version) != self->cfg_.versions.end()) {
                self->current_target_ = self->cfg_.versions.at(version);
                Logger::info("Forwarder: Routing to specific server for {}", version);
            } else {
                self->current_target_ = self->cfg_.default_target;
                Logger::warn("Forwarder: Unknown/New version detected. Routing to DEFAULT server {}:{}", 
                    self->current_target_.host, self->current_target_.port);
            }

            // 3. الاتصال بالسيرفر
            self->connectToTarget(version);
        });
}

void Forwarder::connectToTarget(const std::string& detected_version) {
    auto self = shared_from_this();
    
    resolver_.async_resolve(current_target_.host, std::to_string(current_target_.port),
        [self](asio::error_code ec, asio::ip::tcp::resolver::results_type results) {
            if (ec) { self->closeAll(); return; }
            
            asio::async_connect(self->server_, results,
                [self](asio::error_code ec, const asio::ip::tcp::endpoint&) {
                    if (ec) {
                        Logger::warn("Forwarder: connect failed: {}", ec.message());
                        self->closeAll();
                        return;
                    }
                    // نرسل أول باكت للسيرفر الهدف
                    asio::async_write(self->server_, asio::buffer(self->c2s_, self->c2s_.size()),
                        [self](asio::error_code ec2, std::size_t n) {
                            if (!ec2) self->startRelay();
                            else self->closeAll();
                        });
                });
        });
}

} // namespace Zucks