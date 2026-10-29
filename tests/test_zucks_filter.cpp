#include <gtest/gtest.h>
#include "zucks_filter.hpp"

TEST(ZucksFilterTest, PacketInspectionRejectsEmpty) {
    Zucks::Filter f{Zucks::SecurityConfig{}};
    EXPECT_FALSE(f.inspectPacket(nullptr, 0));
}

TEST(ZucksFilterTest, PacketInspectionAcceptsBasicPacket) {
    Zucks::Filter f{Zucks::SecurityConfig{}};
    uint8_t packet[] = {0xF1, 0x00, 0x08, 0x01, 0x02, 0x03, 0x00, 0x00};
    EXPECT_TRUE(f.inspectPacket(packet, sizeof(packet)));
}

TEST(ZucksFilterTest, RateLimitWorks) {
    Zucks::SecurityConfig cfg;
    cfg.max_connections_per_ip = 2;
    Zucks::Filter f{cfg};

    EXPECT_TRUE(f.allowConnection("192.168.1.10"));
    EXPECT_TRUE(f.allowConnection("192.168.1.10"));
    EXPECT_FALSE(f.allowConnection("192.168.1.10"));
}