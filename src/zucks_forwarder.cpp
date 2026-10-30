#include "zucks_forwarder.hpp"
#include "zucks_protocol_detector.hpp"
#include "zucks_logger.hpp"

namespace Zucks {

Forwarder::Forwarder(asio::ip::tcp::socket client, const Config& cfg, std::shared_ptr<Filter> filter)
    : client_(std::move(client)),
      resolver_(client_.get_executor()),
      server_(client_.get_executor()),
      cfg_(cfg),
      filter_(std::move(filter)) {}

void Forwarder::start() {
    asio::error_code ec;
    const auto ep = client_.remote_endpoint(ec);
    if (ec) { closeAll(); return; }

    const std::string ip = ep.address().to_string();
    if (!filter_->allowConnection(ip)) {
        Logger::warn("Forwarder: connection blocked by filter for {}", ip);
        closeAll();
        return;
    }

    // نبدأ بقراءة أول باكت لنعرف النسخة
    readFirstPacket();
}

void Forwarder::readFirstPacket() {
    auto self = shared_from_this();
    client_.async_read_some(asio::buffer(c2s_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) { self->closeAll(); return; }

            std::string version = ProtocolDetector::detect(
                reinterpret_cast<const uint8_t*>(self->c2s_.data()), n);

            Logger::info("Forwarder: Client connected with version {}", version);

            if (self->cfg_.versions.find(version) == self->cfg_.versions.end()) {
                Logger::warn("Forwarder: Unsupported version {}, closing.", version);
                self->closeAll();
                return;
            }

            self->current_target_ = self->cfg_.versions.at(version);
            self->connectToTarget(version);
        });
}

void Forwarder::connectToTarget(const std::string& detected_version) {
    auto self = shared_from_this();
    Logger::info("Forwarder: Routing {} client to {}:{}", 
        detected_version, current_target_.host, current_target_.port);

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
                    
                    // بمجرد الاتصال، نرسل أول باكت قرأناه مسبقاً
                    asio::async_write(self->server_, asio::buffer(self->c2s_, self->c2s_.size()),
                        [self](asio::error_code ec2, std::size_t n) {
                            if (!ec2) self->startRelay();
                            else self->closeAll();
                        });
                });
        });
}

// (باقي دوال الـ Relay startRelay, relayClientToServer, relayServerToClient, closeAll تبقى كما هي)
void Forwarder::startRelay() {
    relayClientToServer();
    relayServerToClient();
}

void Forwarder::relayClientToServer() {
    auto self = shared_from_this();
    client_.async_read_some(asio::buffer(c2s_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) { self->closeAll(); return; }
            if (!self->filter_->inspectPacket(reinterpret_cast<const uint8_t*>(self->c2s_.data()), n)) {
                self->closeAll(); return;
            }
            asio::async_write(self->server_, asio::buffer(self->c2s_, n),
                [self](asio::error_code ec2, std::size_t) {
                    if (ec2) self->closeAll();
                    else self->relayClientToServer();
                });
        });
}

void Forwarder::relayServerToClient() {
    auto self = shared_from_this();
    server_.async_read_some(asio::buffer(s2c_),
        [self](asio::error_code ec, std::size_t n) {
            if (ec || n == 0) { self->closeAll(); return; }
            asio::async_write(self->client_, asio::buffer(self->s2c_, n),
                [self](asio::error_code ec2, std::size_t) {
                    if (ec2) self->closeAll();
                    else self->relayServerToClient();
                });
        });
}

void Forwarder::closeAll() {
    asio::error_code ec;
    client_.close(ec);
    server_.close(ec);
}

} // namespace Zucks