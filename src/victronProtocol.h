#pragma once

#include <stdint.h>

inline bool victronHexNibble(char c, uint8_t& value) {
    if (c >= '0' && c <= '9') {
        value = static_cast<uint8_t>(c - '0');
        return true;
    }
    if (c >= 'A' && c <= 'F') {
        value = static_cast<uint8_t>(c - 'A' + 10);
        return true;
    }
    if (c >= 'a' && c <= 'f') {
        value = static_cast<uint8_t>(c - 'a' + 10);
        return true;
    }
    return false;
}

inline bool victronDecodeHexByte(char high, char low, uint8_t& value) {
    uint8_t highNibble;
    uint8_t lowNibble;

    if (!victronHexNibble(high, highNibble) ||
        !victronHexNibble(low, lowNibble)) {
        return false;
    }

    value = static_cast<uint8_t>((highNibble << 4) | lowNibble);
    return true;
}

// Victron command frames are valid when their decoded-byte sum is 0x55.
inline bool victronFrameChecksumValid(uint8_t sum) {
    return sum == 0x55;
}