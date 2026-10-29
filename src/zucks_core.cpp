#include "zucks_core.hpp"
#include "zucks_proxy.hpp"
#include "zucks_logger.hpp"
#include <asio.hpp>
#include <csignal>

namespace Zucks {

Core::Core() = default;
Core::~Core() = default;

bool Core::loadConfig(const std::string& path) {
    std::string err;
    if (!Zucks::loadConfig(path, cfg_, err)) {
        Logger::error("{}", err);
        return false;
    }

    Logger::init(cfg_.log_level);
    Logger::info("Zucks config loaded: listen {} -> {}:{}",
                 cfg_.listen_port, cfg_.target_host, cfg_.target_port);
    return true;
}

void Core::run() {
    asio::io_context io;

    proxy_ = std::make_shared<Proxy>(
        io, cfg_.listen_port, cfg_.target_host, cfg_.target_port
    );
    proxy_->start();

    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([this, &io](auto, auto) {
        Logger::info("Shutting down Zucks...");
        proxy_->stop();
        io.stop();
    });

    Logger::info("Zucks Server running");
    io.run();
}

} // namespace Zucks