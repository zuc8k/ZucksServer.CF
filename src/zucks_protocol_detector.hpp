#pragma once
#include <cstdint>
#include <string>
#include <map>

namespace Zucks {

class ProtocolDetector {
public:
    // يكتشف الإصدار بناءً على الخريطة الديناميكية
    static std::string detect(const uint8_t* data, size_t length, const std::map<std::string, std::string>& headersMap);
    
    // لتسجيل الهيدرات المجهولة
    static void logUnknownHeader(uint8_t header);
};

} // namespace Zucks