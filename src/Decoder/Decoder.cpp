#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <string_view>
#include <cstring>
#include "Decoder.h"

Decoder::Decoder(const std::string &hexPayload)
    : payload(hexPayload), currOffset(0), currIndex(0), newOrderCrossMessageFields(hexPayload){}

void Decoder::initiateDecoder() {
    handleStartOfMessage();
    handleNumberOfNewOrderBitfields();
    handleRepeatingGroups();
    handleRepeatingOptionalGroups();
    handleNonRepeatingOptionalGroups();
}

void Decoder::handleStartOfMessage() {
    std::string_view sv(payload);

    uint16_t messageLength = hexLittleToUint16(std::string(sv.substr(4 + currOffset, 4)));
    newOrderCrossMessageFields.setSequenceNumber(messageLength);

    uint32_t sequenceNumber = hexLittleToUint32(std::string(sv.substr(12 + currOffset, 8)));
    newOrderCrossMessageFields.setSequenceNumber(sequenceNumber);

    newOrderCrossMessageFields.setCrossId(hexToChars<20>(sv.substr(20 + currOffset, 40)));

    newOrderCrossMessageFields.setCrossType(hexToChars<1>(sv.substr(60 + currOffset, 2)));

    newOrderCrossMessageFields.setCrossPrioritization(hexToChars<1>(sv.substr(62 + currOffset, 2)));

    uint64_t price = hexLittleToUint64(std::string(sv.substr(64 + currOffset, 16)));
    newOrderCrossMessageFields.setPrice(price);

    uint32_t orderQuantity = hexLittleToUint32(std::string(sv.substr(80 + currOffset, 8)));
    newOrderCrossMessageFields.setOrderQuantity(orderQuantity);

    currIndex += 88;
}

void Decoder::handleNumberOfNewOrderBitfields() {
    std::string_view sv(payload);

    uint8_t numberOfNewOrderCrossBitfields = hexToUint8 (std::string(sv.substr(88, 2)));
    this->currOffset = numberOfNewOrderCrossBitfields * 2;
    newOrderCrossMessageFields.setNumberOfNewOrderCrossBitfields (numberOfNewOrderCrossBitfields);

    std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields = newOrderCrossMessageFields.getNewOrderCrossBitfields();
    for (int i{}; i < numberOfNewOrderCrossBitfields; ++i) {
        uint8_t bitfield = hexToUint8 (std::string(sv.substr(currIndex + i * 2, 2)));
        newOrderCrossBitfields.push_back ({bitfield});
    }
}

void Decoder::handleRepeatingGroups() {

}

void Decoder::handleRepeatingOptionalGroups() {

}

void Decoder::handleNonRepeatingOptionalGroups() {

}

uint8_t Decoder::hexToUint8(const std::string& hex) {
    return static_cast<uint8_t>(std::stoull(hex, nullptr, 16));
}

uint16_t Decoder::hexLittleToUint16(const std::string& hex) {
    uint16_t temp = static_cast<uint16_t>(std::stoul(hex, nullptr, 16));
    return (temp >> 8) | (temp << 8);
}

uint32_t Decoder::hexLittleToUint32(const std::string& hex) {
    uint32_t temp = static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
    return (temp >> 24) | (temp << 24) | ((temp << 8) & 0xFF00) | ((temp >> 8) & 0xFF0000);
}

uint64_t Decoder::hexLittleToUint64(const std::string& hex) {
    uint64_t temp = static_cast<uint64_t>(std::stoull(hex, nullptr, 16));
    return (temp >> 56) | ((temp >> 40) & 0xFF00) | ((temp >> 24) & 0xFF0000) |
           ((temp >> 8) & 0xFF000000) | ((temp << 8) & 0xFF00000000) |
           ((temp << 24) & 0xFF0000000000) | ((temp << 40) & 0xFF000000000000) | (temp << 56);
}