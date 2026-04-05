#pragma once
#include <string>
#include "Messages/NewOrderCrossMsg.h"

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

namespace BitfieldIndex3Values {
    static constexpr BitfieldIndex3 values[] = {
        BitfieldIndex3::CLIENT_ID_ATTR,
        BitfieldIndex3::EQUITY_TRADE_PRICE,
        BitfieldIndex3::EQUITY_TRADE_SIZE,
        BitfieldIndex3::EQUITY_TRADE_VENUE,
        BitfieldIndex3::EQUITY_TRANSACT_TIME,
        BitfieldIndex3::EQUITY_BUY_CLEARING_FIRM,
        BitfieldIndex3::EQUITY_SELL_CLEARING_FIRM,
        BitfieldIndex3::SESSION_ELIGIBILITY
    };
}

namespace BitfieldIndex4Values {
    static constexpr BitfieldIndex4 values[] = {
        BitfieldIndex4::COMPRESSION,
        BitfieldIndex4::ORS,
        BitfieldIndex4::FREQUENT_TRADER_ID
    };
}

class Decoder {
public:
    explicit Decoder(const std::string &hexPayload);
    void initiateDecoder();

private:
    std::vector<uint8_t> matchedBitfields1; //use this for repeating groups at the end in switch statement
    std::vector<uint8_t> matchedBitfields2;
    std::vector<uint8_t> matchedBitfields3;
    std::vector<uint8_t> matchedBitfields4;
    std::vector<uint8_t> includedOptionalBitfields;

    std::string payload;
    size_t currOffset;
    size_t currIndex;
    NewOrderCrossMessageFields newOrderCrossMessageFields;

    void handleStartOfMessage(); //rows 1-10 **beginning message format
    void handleNumberOfNewOrderBitfields(); //rows 11-12 (really n) **bitfields format
    void handleRepeatingGroups(); //13-23 **repeating groups mandatory format
    void handleNonRepeatingOptionalGroups(); //24 **non-repeating optional groups

public:
    NewOrderCrossMessageFields& getNextMessage() {
        return newOrderCrossMessageFields;
    }

};
