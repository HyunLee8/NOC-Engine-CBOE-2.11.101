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
    handleNonRepeatingOptionalGroups();
}

void Decoder::handleStartOfMessage() {
    std::string_view sv(payload);

    uint16_t messageLength = hexLittleToUint16(std::string(sv.substr(4, 4)));
    newOrderCrossMessageFields.setSequenceNumber(messageLength);

    uint32_t sequenceNumber = hexLittleToUint32(std::string(sv.substr(12, 8)));
    newOrderCrossMessageFields.setSequenceNumber(sequenceNumber);


    newOrderCrossMessageFields.setCrossId(hexToChars<20>(sv.substr(20, 40)));

    newOrderCrossMessageFields.setCrossType(hexToChars<1>(sv.substr(60, 2)));

    newOrderCrossMessageFields.setCrossPrioritization(hexToChars<1>(sv.substr(62, 2)));

    uint64_t price = hexLittleToUint64(std::string(sv.substr(64, 16)));
    newOrderCrossMessageFields.setPrice(price);

    uint32_t orderQuantity = hexLittleToUint32(std::string(sv.substr(80, 8)));
    newOrderCrossMessageFields.setOrderQuantity(orderQuantity);

    currIndex += 88;
}

void Decoder::handleNumberOfNewOrderBitfields() {
    std::string_view sv(payload);

    uint8_t numberOfNewOrderCrossBitfields = hexToUint8 (std::string(sv.substr(88, 2)));
    newOrderCrossMessageFields.setNumberOfNewOrderCrossBitfields (numberOfNewOrderCrossBitfields);
    currIndex += 2;

    std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields = newOrderCrossMessageFields.getNewOrderCrossBitfields();
    for (int i{}; i < numberOfNewOrderCrossBitfields; ++i) {
        uint8_t bitfield = hexToUint8(std::string(sv.substr(currIndex, 2)));
        newOrderCrossBitfields.push_back ({bitfield});
        currIndex += 2;
    }
}

void Decoder::handleRepeatingGroups() { //prepare for hell
    std::string_view sv(payload);

    uint16_t groupLength = hexLittleToUint16(std::string(sv.substr(currIndex, 4)));
    newOrderCrossMessageFields.setGroupCount(groupLength);
    currIndex += 4;

    std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields = newOrderCrossMessageFields.getNewOrderCrossBitfields();
    std::vector<uint8_t> includedOptionalBitfields;
    std::vector<uint8_t> matchedBitfields1; //use this for repeating groups at the end in switch statement
    std::vector<uint8_t> matchedBitfields2;
    std::vector<uint8_t> matchedBitfields3;
    std::vector<uint8_t> matchedBitfields4;

    //adds the bitfield values into included optional bitfields
    for (const auto& newOrderCrossBitfield : newOrderCrossBitfields) {
        includedOptionalBitfields.push_back(newOrderCrossBitfield.bitfield);
    }

    //goes through included optioanl bitfields and compares to the real bitfield values and
    //push_back that compared value as matched.
    for (int i{}; i < includedOptionalBitfields.size(); ++i) {
        //bitfield byte 1
        if (i == 0) {
            for (const auto& bitfield : BitfieldIndex1Values::values) {
                if (includedOptionalBitfields.at(i) & static_cast<uint8_t>(bitfield)) {
                    matchedBitfields1.push_back(static_cast<uint8_t>(bitfield));
                }
            }
        } else if (i == 1) { //bitfield byte 2
            for (const auto& bitfield : BitfieldIndex2Values::values) {
                if (includedOptionalBitfields.at(i) & static_cast<uint8_t>(bitfield)) {
                    matchedBitfields2.push_back(static_cast<uint8_t>(bitfield));
                }
            }
        } else if (i == 2) {

        } else {

        }
    }

    std::vector<RepeatingGroup> repeatingGroups;
    for (int i{}; i < groupLength; ++i) {
        char side = hexToChar(std::string(sv.substr(currIndex, 2)));
        repeatingGroups.at(i).side = static_cast<Side>(side);
        currIndex += 4;

        uint32_t allocQuantity = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
        repeatingGroups.at(i).allocQuantity = allocQuantity;
        currIndex += 8;

        std::array<char, StringLength::CLIENT_ORDER_ID> clientOrderId = hexToChars<StringLength::CLIENT_ORDER_ID>(std::string(sv.substr(currIndex, StringLength::CLIENT_ORDER_ID*2)));
        repeatingGroups.at(i).clientOrderId = clientOrderId;
        currIndex += StringLength::CLIENT_ORDER_ID*2;

        char capacity = hexToChar(std::string(sv.substr(currIndex, 2)));
        repeatingGroups.at(i).capacity = static_cast<Capacity>(capacity);
        currIndex += 2;

        char openClose = hexToChar(std::string(sv.substr(currIndex, 2)));
        repeatingGroups.at(i).openClose = static_cast<OpenClose>(openClose);
        currIndex += 2;

        std::array<char, StringLength::GIVE_UP_FIRM_ID> giveUpFirmId = hexToChars<StringLength::GIVE_UP_FIRM_ID>(std::string(sv.substr(currIndex, StringLength::GIVE_UP_FIRM_ID*2)));
        repeatingGroups.at(i).giveUpFirmId = giveUpFirmId;
        currIndex += (StringLength::GIVE_UP_FIRM_ID*2);

        //FILL in optional bitfields
        for (const auto& bitfield : matchedBitfields2) {
            switch (bitfield) {
                case static_cast<uint8_t>(BitfieldIndex2::ACCOUNT): {
                    std::array<char, StringLength::ACCOUNT> account = hexToChars<StringLength::ACCOUNT>(std::string(sv.substr(currIndex, StringLength::ACCOUNT*2)));
                    repeatingGroups.at(i).repeatingOptionalBitfields.account = account;
                    currIndex += (StringLength::ACCOUNT*2);
                    break;
                }
                case static_cast<uint8_t>(BitfieldIndex2::CMTA_NUMBER): {
                    uint32_t cmtaNumber = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
                    repeatingGroups.at(i).repeatingOptionalBitfields.cmtaNumber = cmtaNumber;
                    currIndex += 8;
                    break;
                }
                case static_cast<uint8_t>(BitfieldIndex2::CLEARING_ACCOUNT): {
                    uint32_t clearingAccount = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
                    repeatingGroups.at(i).repeatingOptionalBitfields.clearingAccount = clearingAccount;
                    currIndex += 8;
                    break;
                }
                case static_cast<uint8_t>(BitfieldIndex2::CLEARING_OPTIONAL_DATA): {
                    std::array<char, StringLength::CLEARING_OPTIONAL_DATA> clearingOptionalData = hexToChars<StringLength::CLEARING_OPTIONAL_DATA>(std::string(sv.substr(currIndex, StringLength::CLEARING_OPTIONAL_DATA*2)));
                    repeatingGroups.at(i).repeatingOptionalBitfields.clearingOptionalData = clearingOptionalData;
                    currIndex += (StringLength::CLEARING_OPTIONAL_DATA*2);
                    break;
                }
            }
        }

        //need this in seperate loop because it's Byte 4 index
        for (const auto& bitfield : matchedBitfields4) {
            if (bitfield == static_cast<uint8_t>(BitfieldIndex4::FREQUENT_TRADER_ID)) {
                std::array<char, StringLength::FREQUENT_TRADER_ID> frequentTraderId = hexToChars<StringLength::FREQUENT_TRADER_ID>(std::string(sv.substr(currIndex, StringLength::FREQUENT_TRADER_ID*2)));
                repeatingGroups.at(i).repeatingOptionalBitfields.frequentTraderId = frequentTraderId;
                currIndex += (StringLength::FREQUENT_TRADER_ID*2);
                break;
            }
        }
    }
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

char Decoder::hexToChar(std::string_view hex) {
    return static_cast<char>(std::stoul(std::string(hex), nullptr, 16));
}