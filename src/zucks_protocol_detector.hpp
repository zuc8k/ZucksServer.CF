// Zucks Server - Protocol Detector
#pragma once
#include <cstdint>
#include <string>

namespace Zucks {

class ProtocolDetector {
public:
    // يكتشف الإصدار ويرجع "1.1.0" أو "2.1.0" أو "3.1.0" أو "Unknown"
    static std::string detect(const uint8_t* data, size_t length);
};

} // namespace Zucks