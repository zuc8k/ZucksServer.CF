// Zucks Server - Protocol Detector Implementation
// (c) 2026 Zucks Project. All rights reserved.

#include "zucks_protocol_detector.hpp"
#include "zucks_logger.hpp"

namespace Zucks {

CFVersion ProtocolDetector::detect(const uint8_t* data, size_t length) {
    if (!data || length < 4) {
        return CFVersion::Unknown;
    }

    // التقريب المبدئي بناءً على أول بايت
    // (يمكن تعديل الأرقام دي حسب هيكل الباكت الحقيقي لنسخك)
    if (data[0] == CF_V1_MAGIC) return CFVersion::V1;
    if (data[0] == CF_V2_MAGIC) return CFVersion::V2;
    if (data[0] == CF_V3_MAGIC) return CFVersion::V3;

    // لو مفيش Magic واضح، ممكن نستخدم طول الباكت أو شكل الـ Header
    // كـ fallback (مثلاً v1 بيكون 2 بايت طول، v2 بيكون 4 بايت)
    return CFVersion::Unknown;
}

const char* ProtocolDetector::versionName(CFVersion v) {
    switch (v) {
        case CFVersion::V1: return "CrossFire v1 (Legacy)";
        case CFVersion::V2: return "CrossFire v2 (Stable)";
        case CFVersion::V3: return "CrossFire v3 (CN/Modern)";
        default:            return "Unknown / Mixed";
    }
}

bool ProtocolDetector::isValidPacket(CFVersion v, const uint8_t* data, size_t length) {
    if (!data || length < 4) return false;

    // هنا تحط قواعد الفلتر الخاصة بكل إصدار
    switch (v) {
        case CFVersion::V1:
            // v1 غالباً بيكون باكتات صغيرة، ومفيش تشفير معقد
            return length <= 6000; 

        case CFVersion::V2:
            // v2 بيدعم باكتات أكبر شوية
            return length <= 8192;

        case CFVersion::V3:
            // v3 ممكن يكون فيه ضغط أو شغل إضافي
            return length > 0 && length <= 16384;

        default:
            return false;
    }
}

} // namespace Zucks