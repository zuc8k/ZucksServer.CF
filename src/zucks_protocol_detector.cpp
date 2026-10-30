#include "zucks_protocol_detector.hpp"
#include "zucks_logger.hpp"
#include <iomanip>
#include <sstream>
#include <fstream>

namespace Zucks {

std::string ProtocolDetector::detect(const uint8_t* data, size_t length, const std::map<std::string, std::string>& headersMap) {
    if (!data || length < 4) return "Unknown";

    uint8_t header = data[0];

    // تحويل البايت إلى شكل Hex (مثال: 0x01)
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(header);
    std::string hexStr = ss.str();

    // البحث في الخريطة
    auto it = headersMap.find(hexStr);
    if (it != headersMap.end()) {
        return it->second;
    }

    // لو مجهول، نسجله عشان المطور يعرفه
    logUnknownHeader(header);
    return "Unknown";
}

void ProtocolDetector::logUnknownHeader(uint8_t header) {
    std::ofstream log("unknown_headers.log", std::ios::app);
    if (log) {
        log << "Detected Unknown Header: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(header) << "\n";
    }
}

} // namespace Zucks