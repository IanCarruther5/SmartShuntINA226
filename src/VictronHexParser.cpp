#include "VictronHexParser.h"

namespace victron {

HexParser::HexParser() {
    reset();
}

void HexParser::reset() {
    state_ = State::Idle;
    checksum_ = 0;
    index_ = 0;
    packet_ = {};
}

bool HexParser::nibble(uint8_t c, uint8_t& out) {
    if (c >= '0' && c <= '9') { out = c - '0'; return true; }
    if (c >= 'A' && c <= 'F') { out = c - 'A' + 10; return true; }
    if (c >= 'a' && c <= 'f') { out = c - 'a' + 10; return true; }
    return false;
}

bool HexParser::hexToByte(uint8_t h, uint8_t l, uint8_t& result) {
    uint8_t hi, lo;
    if (!nibble(h, hi) || !nibble(l, lo)) return false;
    result = (hi << 4) | lo;
    return true;
}

ParseResult HexParser::input(uint8_t byte, Packet& outPacket) {

    switch (state_) {

        case State::Idle:
            if (byte == ':') {
                reset();
                state_ = State::ReadCommand;
            }
            return ParseResult::InProgress;

        case State::ReadCommand:
            packet_.command = static_cast<Command>(byte - '0');
            checksum_ += static_cast<uint8_t>(packet_.command);
            state_ = State::ReadRegister;
            return ParseResult::InProgress;

        case State::ReadRegister:
            packet_.address <<= 8;
            packet_.address |= byte;
            checksum_ += byte;
            state_ = State::ReadFlags;
            return ParseResult::InProgress;

        case State::ReadFlags:
            packet_.flags = byte;
            checksum_ += byte;
            state_ = State::ReadData;
            return ParseResult::InProgress;

        case State::ReadData:
            if (byte == '\n') {
                if (checksum_ == CHECKSUM_TARGET) {
                    outPacket = packet_;
                    reset();
                    return ParseResult::Complete;
                } else {
                    reset();
                    return ParseResult::Error;
                }
            }

            if (index_ >= MAX_DATA_SIZE) {
                reset();
                return ParseResult::Error;
            }

            packet_.data[index_++] = byte;
            packet_.length = index_;
            checksum_ += byte;
            return ParseResult::InProgress;

        default:
            reset();
            return ParseResult::Error;
    }
}

}
