// Zucks Server - Unit Tests for Filter
// (c) 2026 Zucks Project. All rights reserved.

#include <gtest/gtest.h>
#include "zucks_filter.hpp"

TEST(ZucksFilterTest, PacketInspection) {
    Zucks::Filter filter;
    uint8_t valid_packet[] = {0xF1, 0x00, 0x08, 0x01, 0x02, 0x03, 0x00, 0x00};
    EXPECT_TRUE(filter.inspectPacket(valid_packet, sizeof(valid_packet)));
}

TEST(ZucksFilterTest, RateLimiting) {
    Zucks::Filter filter;
    EXPECT_TRUE(filter.allowConnection("192.168.1.1"));
    // بعد تجاوز الحد، يجب أن يعيد false
}