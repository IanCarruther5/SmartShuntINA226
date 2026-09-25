#include <unity.h>
#include "../src/victronProtocol.h"

void test_victron_decodes_hex_byte() {
    uint8_t value = 0;

    TEST_ASSERT_TRUE(victronDecodeHexByte('A', 'f', value));
    TEST_ASSERT_EQUAL_UINT8(0xAF, value);

    TEST_ASSERT_TRUE(victronDecodeHexByte('0', '9', value));
    TEST_ASSERT_EQUAL_UINT8(0x09, value);
}

void test_victron_rejects_non_hex() {
    uint8_t value = 0;

    TEST_ASSERT_FALSE(victronDecodeHexByte('G', '0', value));
    TEST_ASSERT_FALSE(victronDecodeHexByte('0', ':', value));
}

void test_victron_frame_checksum_validation() {
    TEST_ASSERT_TRUE(victronFrameChecksumValid(0x55));
    TEST_ASSERT_FALSE(victronFrameChecksumValid(0x54));
}