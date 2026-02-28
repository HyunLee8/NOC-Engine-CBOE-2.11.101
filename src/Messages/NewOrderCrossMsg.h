#pragma once
#include <vector>
#include <variant>
#include <chrono>
#include <array>
#include <unordered_map>

namespace StringLength {
    constexpr size_t CROSS_ID = 20;
    constexpr size_t CROSS_TYPE = 1;
    constexpr size_t CROSS_PRIORITIZATION = 1;
    constexpr size_t SIDE = 1;
    constexpr size_t CLIENT_ORDER_ID = 20;
    constexpr size_t CAPACITY = 1;
    constexpr size_t OPEN_CLOSE = 1;
    constexpr size_t GIVE_UP_FIRM_ID = 4;
    constexpr size_t ACCOUNT = 16;
    constexpr size_t CLEARING_ACCOUNT = 4;
    constexpr size_t CLEARING_OPTIONAL_DATA = 16;
    constexpr size_t FREQUENT_TRADER_ID = 6;
    constexpr size_t SYMBOL = 8;
    constexpr size_t TARGET_PARTY_ID = 4;
    constexpr size_t PREVENT_MATCH = 3;
    constexpr size_t ROUTING_FIRM_ID = 4;
}

enum class CrossTypes : char {
    AIM = '1',
    QCC = '2',
    SAM = '3',
    PCC = '4'
};

enum class CrossPrioritization : char {
    BUY = '1',
    SELL = '2'
};

enum class Side : char {
    BUY = '1',
    SELL = '2'
};

enum class Capacity : char {
    CUSTOMER = 'C',
    MARKET_MAKER = 'M',
    FIRM = 'F',
    PROFESSIONAL_CUSTOMER = 'U',
    AWAYMARKET_MAKER = 'N',
    BROKER_DEALER = 'B',
    JOINT_BACK_OFFICE = 'J'
};

enum class OpenClose : char {
    OPEN = 'O',
    CLOSE = 'C',
    NONE = 'N'
};

enum class PutOrCall : char {
    PUT = '0',
    CALL = '1'
};

enum class AttributedQuote : char {
    NO_FIRM_ID = 'N',
    ATTRIBUTE_FIRM_ID = 'Y',
    CLIENT_ID_ONLY = 'C',
    CLEARING_CLIENT = 'Z'
};

enum class PreventMatch : char {
    NONE = '0'
};

enum class BitfieldIndex1 : size_t {
    SYMBOL = 1,
    MATURITY_DATE = 2,
    STRIKE_PRICE = 4,
    PUT_OR_CALL = 8,
    EXEC_INSTRUCTION = 16,
    ATTRIBUTED_QUOTE = 32,
    TARGET_PARTY_ID = 64,
    PREVENT_MATCH = 128
};

enum class BitfieldIndex2 : size_t {
    AUTO_MATCH = 1,
    AUTO_MATCH_PRICE = 2,
    LAST_PRIORITY = 4,
    ACCOUNT = 8,
    CMTA_NUMBER = 16,
    CLEARING_ACCOUNT = 32,
    ROUTING_FIRM_ID = 64,
    CLEARING_OPTIONAL_DATA = 128
};

enum class AutoMatch : char {
    DISABLED = '0',
    MARKET = '1',
    LIMIT = '2',
    MARKET_NA = '3',
    LIMIT_NA = '4'
};

enum class LastPriority : char {
    DISABLED = '0',
    ENABLED = '1',
};

enum class BitfieldIndex3 : size_t {
    NONE = 0
};

enum class BitfieldIndex4 : size_t {
    NONE = 0
};

struct NewOrderCrossBitfield {
    size_t indexNum;
    uint16_t bitfield;
};

struct RepeatingOptionalBitfield {
    std::array<char, StringLength::ACCOUNT> account;
    uint32_t cmtaNumber;
    uint32_t clearingAccount;
    std::array<char, StringLength::CLEARING_OPTIONAL_DATA> clearingOptionalData;
    std::array<char, StringLength::FREQUENT_TRADER_ID> frequentTraderId;
};

struct RepeatingGroup {
    Side side;
    uint32_t allocQuantity;
    std::array<char, StringLength::CLIENT_ORDER_ID> clientOrderId;
    Capacity capacity;
    OpenClose openClose;
    std::array<char, StringLength::GIVE_UP_FIRM_ID> giveUpFirmId;

    // if the bitfields bit is 1 for the following:
    // account, cmtaNum, clearing acc, clearing opt data, frequent traderID, then it is repeating
    // If this group is not accounced in number of bitfields
    // then DON'T pad shit. Just don;t add and order that don't
    // have a speicfic bitfield even though it was ivluded just pad that jaunt

    RepeatingOptionalBitfield repeatingOptionalBitfields;
};

struct NonRepeatingOptionalField {
    //bitfield byte #1
    std::array<char, StringLength::SYMBOL> symbol;
    std::string maturityDate;
    uint64_t strikePrice;
    PutOrCall putOrCall;
    char execInst;
    AttributedQuote attributedQuote;
    std::array<char, StringLength::TARGET_PARTY_ID> targetPartyId;
    PreventMatch preventMatch;

    //bitfield byte #2
    //LAST PICKUP HERE ^^^^
    AutoMatch autoMatch;
    uint64_t autoMatchPrice;
    LastPriority lastPriority;
    std::array<char, StringLength::ACCOUNT> account;
    uint32_t cmtaNumber;
    std::array<char, StringLength::CLEARING_ACCOUNT> clearingAccount;
    std::array<char, StringLength::ROUTING_FIRM_ID> routingFirmId;
};

class NewOrderCrossMessageFields {
private:
    const uint16_t startOfMessage = 47802;
    uint16_t messageLength;
    const uint16_t messageType = 65;
    const uint16_t matchingUnit = 0;
    uint32_t sequenceNumber;
    std::array<char, StringLength::CROSS_ID> crossId;
    std::array<char, StringLength::CROSS_TYPE> crossType;
    std::array<char, StringLength::CROSS_PRIORITIZATION> crossPrioritization;
    uint64_t price;
    uint32_t orderQuantity;

    uint8_t numberOfNewOrderCrossBitfields; //DIRECTLY COUNTS AMOUNT OF OPTIONAL FIELDS AT THE END. Some bitfields are repeating groups and others aren't
    std::vector<NewOrderCrossBitfield> newOrderCrossBitfields;

    //repeating fields w/ optional fields | must be includded
    uint16_t groupCount;
    std::vector<RepeatingGroup> repeatingGroups;

    //Non-repeating Optional Fields.
    //name is a bit misleading since Bitfields still
    //include repeating fields but did it for the sake
    //for organization cuz I may reuse Bitfields but for
    //now just know that Bitfields has stuff ilke
    //account which can be repeating
    NonRepeatingOptionalField nonRepeatingOptionalFields;

public:
    NewOrderCrossMessageFields(const std::string &payload);

private:
    const std::string stringPayload;
};




