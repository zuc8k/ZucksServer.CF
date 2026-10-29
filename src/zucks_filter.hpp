#pragma once
#include "zucks_config.hpp"
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <chrono>

namespace Zucks {

class Filter {
public:
    explicit Filter(SecurityConfig cfg);

    bool inspectPacket(const uint8_t* data, size_t length);
    bool allowConnection(const std::string& ip);
    void blacklistIP(const std::string& ip);

private:
    struct Bucket {
        double tokens = 0.0;
        std::chrono::steady_clock::time_point last{};
    };

    SecurityConfig cfg_;
    std::mutex mutex_;
    std::unordered_map<std::string, Bucket> buckets_;
    std::unordered_map<std::string, bool> blacklist_;
};

} // namespace Zucks