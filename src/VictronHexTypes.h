#pragma once
#include <stdint.h>
#include <cstddef>

namespace victron {

static constexpr uint8_t CHECKSUM_TARGET = 0x55;
static constexpr size_t MAX_DATA_SIZE = 64;

enum class Command : uint8_t {
    Ping = 1,
    AppVersion = 3,
    ProductId = 4,
    Restart = 6,
    Get = 7,
    Set = 8,
    Async = 0xA,
    Unknown = 0xFF
};

enum class ParseResult {
    InProgress,
    Complete,
    Error
};

struct Packet {
    Command command = Command::Unknown;
    uint16_t address = 0;
    uint8_t flags = 0;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t length = 0;
};

}