#include "zucks_core.hpp"
#include "zucks_logger.hpp"
#include <exception>
#include <string>

int main(int argc, char* argv[]) {
    Zucks::Logger::init("info");
    Zucks::Logger::info("Starting Zucks Server");

    const std::string configPath =
        (argc > 1) ? argv[1] : "config/zucks_config.json";

    try {
        Zucks::Core server;
        if (!server.loadConfig(configPath)) return 1;
        server.run();
    } catch (const std::exception& e) {
        Zucks::Logger::critical("Fatal error: {}", e.what());
        return 1;
    }

    Zucks::Logger::info("Zucks Server shutdown complete");
    return 0;
}