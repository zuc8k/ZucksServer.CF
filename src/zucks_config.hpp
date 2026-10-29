#pragma once
#include <cstdint>
#include <string>

namespace Zucks {

struct SecurityConfig {
    uint32_t max_pps = 1500;
    uint64_t max_bps = 5 * 1024 * 1024;
    uint32_t max_connections_per_ip = 30;
    bool blacklist_enabled = true;
};

struct Config {
    std::string version = "1.1.0";
    std::string log_level = "info";
    uint16_t listen_port = 28888;
    std::string target_host = "127.0.0.1";
    uint16_t target_port = 18889;
    SecurityConfig security;
};

bool loadConfig(const std::string& path, Config& out, std::string& err);

} // namespace Zucks