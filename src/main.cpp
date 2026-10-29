// Zucks Server - Main Entry Point
// (c) 2026 Zucks Project. All rights reserved.

#include "zucks_core.hpp"
#include "zucks_logger.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Zucks::Logger::init();
    Zucks::Logger::info("Starting Zucks Server v1.0.0");

    try {
        Zucks::Core server;
        if (!server.loadConfig("config/zucks_config.json")) {
            Zucks::Logger::error("Failed to load configuration");
            return 1;
        }
        server.run();
    } catch (const std::exception& e) {
        Zucks::Logger::critical("Fatal error: {}", e.what());
        return 1;
    }

    Zucks::Logger::info("Zucks Server shutdown complete");
    return 0;
}