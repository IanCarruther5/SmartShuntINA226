#pragma once
#include "VictronHexTypes.h"

namespace victron {

class HexParser {
public:
    HexParser();

    ParseResult input(uint8_t byte, Packet& outPacket);
    void reset();

private:
    enum class State : uint8_t {
        Idle,
        ReadCommand,
        ReadRegister,
        ReadFlags,
        ReadData,
        ReadChecksum,
        Complete
    };

    State state_;
    Packet packet_;
    uint8_t checksum_;
    uint8_t index_;

    bool hexToByte(uint8_t high, uint8_t low, uint8_t& result);
    bool nibble(uint8_t c, uint8_t& out);
};

}