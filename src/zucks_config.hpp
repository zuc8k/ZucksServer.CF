#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>

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
    std::string game_path = ""; // مسار اللعبة لقراءته تلقائياً
    
    TargetConfig default_target; // السيرفر الافتراضي
    std::unordered_map<std::string, TargetConfig> versions;
    std::map<std::string, std::string> headers_map; // يربط الهيدر (0x01) بالإصدار (1.1.0)
    SecurityConfig security;
};

bool loadConfig(const std::string& path, Config& out, std::string& err);
std::string readGameVersionFromIni(const std::string& game_path);

} // namespace Zucks