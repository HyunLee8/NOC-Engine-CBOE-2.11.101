#pragma once
#include <vector>
#include <variant>
#include <chrono>
#include <unordered_map>
#include "StrongType.h"

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
//Figure out alter this shit is important
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

enum class BitfieldIndex3 : size_t {

};

enum class BitfieldIndex4 : size_t {
    
};


namespace NewOrderCrossMessageFieldConstants {
    constexpr uint16_t START_OF_MESSAGE = 47802;
    constexpr uint16_t MESSAGE_TYPE = 65;
    constexpr uint16_t MATCHIG_UNIT = 0;
}

namespace NewOrderCrossMessageFieldDynamics {
    uint16_t MESSAGE_LENGTH;
    uint8_t MATCHING_UNIT;
    uint32_t SEQUENCE_NUMBER;
    char CROSS_ID[StringLength::CROSS_ID];
    CrossTypes CROSS_TYPE[StringLength::CROSS_TYPE];
    CrossPrioritization CROSS_PRIORITIZATION[StringLength::CROSS_PRIORITIZATION];
    uint64_t PRICE;
    uint32_t ORDER_QTY;

    uint8_t NUMBER_OF_NEW_ORDER_CROSS_BITFIELDS; //DIRECTLY COUNTS AMOUNT OF OPTIONAL FIELDS AT THE END. Some bitfields are repeating groups and others aren't
    std::vector<NewOrderCrossBitfield> NEW_ORDER_CROSS_BITFIELDS;

    //repeating fields w/ optional fields | must be includded
    uint16_t GROUP_COUNT;
    std::vector<RepeatingGroup> REPEATING_GROUPS;

    //Non-repeating Optional Fields.
    //name is a bit misleading since Bitfields still
    //include repeating fields but did it for the sake
    //for organization cuz I may reuse Bitfields but for
    //now just know that Bitfields has stuff ilke
    //account which can be repeating
    NonRepeatingOptionalField nonRepeatingOptionalFields;
};

struct NewOrderCrossBitfield {
    size_t indexNum;
    uint16_t bitfield;
};

struct RepeatingGroup {
    Side side;
    uint32_t allocQuantity;
    char clientOrderId[StringLength::CLIENT_ORDER_ID];
    Capacity capacity;
    OpenClose openClose;
    char giveUpFirmId[StringLength::GIVE_UP_FIRM_ID];

    // if the bitfields bit is 1 for the following:
    // account, cmtaNum, clearing acc, clearing opt data, frequent traderID, then it is repeating
    // If this group is not accounced in number of bitfields
    // then DON'T pad shit. Just don;t add and order that don't
    // have a speicfic bitfield even though it was ivluded just pad that jaunt
    RepeatingOptionalBitfield repeatingOptionalBitfields;

};

struct RepeatingOptionalBitfield {
    char account[16];
    uint32_t cmtaNumber;
    uint32_t clearingAccount;
    char clearingOptionalData[StringLength::CLEARING_OPTIONAL_DATA];
    char frequentTraderId[StringLength::FREQUENT_TRADER_ID]; 
};

struct NonRepeatingOptionalField {
    //bitfield byte #1
    char symbol[StringLength::SYMBOL];
    std::string maturityDate;
    uint64_t strikePrice;
    PutOrCall putOrCall;
    char execInst;
    AttributedQuote attributedQuote;
    char targetPartyId[StringLength::TARGET_PARTY_ID];
    PreventMatch preventMatch;

    //bitfield byte #2
    //LAST PICKUP HERE ^^^^

};

class NewOrderCrossMessageField {
private:
public:

};




