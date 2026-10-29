#include "zucks_filter.hpp"

namespace Zucks {

Filter::Filter(SecurityConfig cfg) : cfg_(cfg) {}

bool Filter::inspectPacket(const uint8_t* data, size_t length) {
    if (!data || length == 0) return false;
    if (length < 4) return false;

    // قواعد مبدئية قابلة للتوسع حسب بروتوكول CF
    // مثال: رفض الحزم ذات البادئة غير المعروفة
    // يمكن إضافة DPI rules هنا لاحقًا.
    return true;
}

bool Filter::allowConnection(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (cfg_.blacklist_enabled) {
        auto it = blacklist_.find(ip);
        if (it != blacklist_.end() && it->second) return false;
    }

    const auto now = std::chrono::steady_clock::now();
    auto& b = buckets_[ip];

    if (b.last.time_since_epoch().count() == 0) {
        b.tokens = static_cast<double>(cfg_.max_connections_per_ip);
        b.last = now;
    }

    const double elapsedSec =
        std::chrono::duration<double>(now - b.last).count();
    const double refillPerSec =
        static_cast<double>(cfg_.max_connections_per_ip) / 60.0;

    b.tokens = std::min<double>(
        static_cast<double>(cfg_.max_connections_per_ip),
        b.tokens + elapsedSec * refillPerSec
    );
    b.last = now;

    if (b.tokens < 1.0) return false;

    b.tokens -= 1.0;
    return true;
}

void Filter::blacklistIP(const std::string& ip) {
    std::lock_guard<std::mutex> lock(mutex_);
    blacklist_[ip] = true;
}

} // namespace Zucks