// Zucks Server - Config Module
#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace Zucks {

struct TargetConfig {
    std::string host;
    uint16_t port;
};

struct SecurityConfig {
    uint32_t max_connections_per_ip = 30;
    bool blacklist_enabled = true;
};

struct Config {
    std::string log_level = "info";
    uint16_t listen_port = 28888;
    std::unordered_map<std::string, TargetConfig> versions;
    SecurityConfig security;
};

bool loadConfig(const std::string& path, Config& out, std::string& err);

} // namespace Zucks