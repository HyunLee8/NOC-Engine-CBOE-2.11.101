#pragma once
#include <string>
#include "../Messages/NewOrderCrossMsg.h"


namespace BitfieldIndex1Values {
    static constexpr BitfieldIndex1 values[] = {
        BitfieldIndex1::SYMBOL,
        BitfieldIndex1::MATURITY_DATE,
        BitfieldIndex1::STRIKE_PRICE,
        BitfieldIndex1::PUT_OR_CALL,
        BitfieldIndex1::EXEC_INSTRUCTION,
        BitfieldIndex1::ATTRIBUTED_QUOTE,
        BitfieldIndex1::TARGET_PARTY_ID,
        BitfieldIndex1::PREVENT_MATCH
    };
}

namespace BitfieldIndex2Values {
    static constexpr BitfieldIndex2 values[] = {
        BitfieldIndex2::AUTO_MATCH,
        BitfieldIndex2::AUTO_MATCH_PRICE,
        BitfieldIndex2::LAST_PRIORITY,
        BitfieldIndex2::ACCOUNT,
        BitfieldIndex2::CMTA_NUMBER,
        BitfieldIndex2::CLEARING_ACCOUNT,
        BitfieldIndex2::ROUTING_FIRM_ID,
        BitfieldIndex2::CLEARING_OPTIONAL_DATA
    };
}

class Decoder {
public:
    Decoder(const std::string &hexPayload);
    void initiateDecoder();

private:
    std::string payload;
    size_t currIndex;
    size_t currOffset;
    NewOrderCrossMessageFields newOrderCrossMessageFields;

    void handleStartOfMessage(); //rows 1-10 **beginning message format
    void handleNumberOfNewOrderBitfields(); //rows 11-12 (really n) **bitfields format
    void handleRepeatingGroups(); //13-23 **repeating groups mandatory format
    void handleNonRepeatingOptionalGroups(); //24 **non-repeating optional groups

    uint8_t hexToUint8(const std::string &hex);
    uint16_t hexLittleToUint16(const std::string &hex);
    uint32_t hexLittleToUint32(const std::string &hex);
    uint64_t hexLittleToUint64(const std::string &hex);


    template<size_t N>
    std::array<char, N> hexToChars(std::string_view hex) {
        std::array<char, N> arr;
        for (size_t i = 0; i < N; i++) {
            std::string byte(hex.substr(i * 2, 2)); // small temporary
            arr[i] = static_cast<char>(std::stoul(byte, nullptr, 16));
        }
        return arr;
    }

    char hexToChar(std::string_view hex);

};
