// Zucks Server - Protocol Detector (CF v1 / v2 / v3)
// (c) 2026 Zucks Project. All rights reserved.

#pragma once

#include <cstdint>
#include <string>
#include <array>

namespace Zucks {

enum class CFVersion {
    Unknown = 0,
    V1      = 1,
    V2      = 2,
    V3      = 3
};

class ProtocolDetector {
public:
    // بنقرأ أول 4 بايت من الاتصال
    // عشان نعرف العميل بيتكلم إصدار كام
    static CFVersion detect(const uint8_t* data, size_t length);
    
    // اسم الإصدار (للتسجيل)
    static const char* versionName(CFVersion v);
    
    // التحقق إن الباكيت متوافق مع قواعد الإصدار
    static bool isValidPacket(CFVersion v, const uint8_t* data, size_t length);

private:
    // رؤوس مميزة لكل إصدار (بناءً على البايتات الأولى)
    static constexpr uint8_t CF_V1_MAGIC = 0xF1;
    static constexpr uint8_t CF_V2_MAGIC = 0xF2;
    static constexpr uint8_t CF_V3_MAGIC = 0xF3;
};

} // namespace Zucks