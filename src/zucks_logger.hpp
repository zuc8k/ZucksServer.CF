// Zucks Server - Logger Module
// (c) 2026 Zucks Project. All rights reserved.

#pragma once
#include <spdlog/spdlog.h>

namespace Zucks::Logger {

inline void init() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [Zucks] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info);
}

template<typename... Args>
inline void info(const char* fmt, Args&&... args) {
    spdlog::info("[Zucks] " + std::string(fmt), std::forward<Args>(args)...);
}

// ... دوال مشابهة لـ error, critical, إلخ.

} // namespace Zucks::Logger