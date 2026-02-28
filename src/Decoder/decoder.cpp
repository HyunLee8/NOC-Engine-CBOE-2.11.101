#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <string_view>
#include "decoder.h"

Decoder::Decoder(const std::string &hexPayload)
    : payload(hexPayload), newOrderCrossMessageFields(hexPayload){}

void Decoder::initiateDecoder() {
    handleStartOfMessage();
    handleNumberOfNewOrderBitfields();
    handleRepeatingGroups();
    handleRepeatingOptionalGroups();
    handleNonRepeatingOptionalGroups();
}

void Decoder::handleStartOfMessage() {
    std::string_view sv(payload);
    uint16_t messageLength = hexLittleToUint16(std::string(sv.substr(4, 4)));
}

void Decoder::handleNumberOfNewOrderBitfields() {

}

void Decoder::handleRepeatingGroups() {

}

void Decoder::handleRepeatingOptionalGroups() {

}

void Decoder::handleNonRepeatingOptionalGroups() {

}

uint16_t Decoder::hexLittleToUint16(const std::string& hex) {
    uint16_t temp = static_cast<uint16_t>(std::stoul(hex, nullptr, 16));
    return (temp >> 8) | (temp << 8);
}

