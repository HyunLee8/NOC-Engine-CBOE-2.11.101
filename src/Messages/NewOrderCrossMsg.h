#pragma once
#include <vector>
#include <chrono>
#include <array>
#include <nlohmann/json.hpp>

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
    constexpr size_t CLIENT_ID_ATTR = 4;
    constexpr size_t EQUITY_BUY_CLEARING_FIRM = 4;
    constexpr size_t EQUITY_SELL_CLEARING_FIRM = 4;
    constexpr size_t ORS = 1;
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

enum class BitfieldIndex1 : uint8_t {
    SYMBOL = 1,
    MATURITY_DATE = 2,
    STRIKE_PRICE = 4,
    PUT_OR_CALL = 8,
    EXEC_INSTRUCTION = 16,
    ATTRIBUTED_QUOTE = 32,
    TARGET_PARTY_ID = 64,
    PREVENT_MATCH = 128
};

enum class BitfieldIndex2 : uint8_t {
    AUTO_MATCH = 1,
    AUTO_MATCH_PRICE = 2,
    LAST_PRIORITY = 4,
    ACCOUNT = 8,
    CMTA_NUMBER = 16,
    CLEARING_ACCOUNT = 32,
    ROUTING_FIRM_ID = 64,
    CLEARING_OPTIONAL_DATA = 128
};

enum class BitfieldIndex3 : uint8_t {
    CLIENT_ID_ATTR = 1,
    EQUITY_TRADE_PRICE = 2,
    EQUITY_TRADE_SIZE = 4,
    EQUITY_TRADE_VENUE = 8,
    EQUITY_TRANSACT_TIME = 16,
    EQUITY_BUY_CLEARING_FIRM = 32,
    EQUITY_SELL_CLEARING_FIRM = 64,
    SESSION_ELIGIBILITY = 128
};

enum class BitfieldIndex4 : uint8_t {
    COMPRESSION = 1,
    ORS = 2,
    FREQUENT_TRADER_ID = 4
    //rest are just reserve bits and not used
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

enum class EquityTradeVenue : char {
    NYSE_AMERICAN = 'A',
    NASDAQ_TEXAS = 'B',
    NYSE_NATIONAL = 'C',
    INVESTORS_EXCHANGE = 'I',
    CBOE_EDGA_EXCHANGE = 'J',
    CBOE_EDGX_EXCHANGE = 'K',
    CHX = 'M',
    NYSE = 'N',
    NYSE_ARCO = 'P',
    NASDAQ = 'Q',
    NASDAQ_PSX = 'X',
    CBOE_BYX_EXCHANGE = 'Y',
    CBOE_BZX_EXCHANGE = 'Z'
};

enum class SessionEligibility : char {
    REGULAR = 'R',
    GLOBAL_REGULAR = 'A',
    RTH_CURB_SESSION = 'B'
};

enum class Compression : char {
    NO = 'N',
    YES = 'Y'
};

struct NewOrderCrossBitfield {
    uint16_t bitfield;
};

struct RepeatingOptionalBitfield {
    std::array<char, StringLength::ACCOUNT> account;
    std::optional<uint32_t>  cmtaNumber;
    std::optional<uint32_t> clearingAccount;
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
    std::optional<uint32_t> maturityDate;
    std::optional<uint64_t> strikePrice;
    std::optional<PutOrCall> putOrCall;
    std::optional<char> execInst;
    std::optional<AttributedQuote> attributedQuote;
    std::array<char, StringLength::TARGET_PARTY_ID> targetPartyId;
    std::array<char, StringLength::PREVENT_MATCH> preventMatch;

    //bitfield byte #2
    //LAST PICKUP HERE ^^^^
    std::optional<AutoMatch> autoMatch;
    std::optional<uint64_t> autoMatchPrice;
    std::optional<LastPriority> lastPriority;
    std::array<char, StringLength::ROUTING_FIRM_ID> routingFirmId;

    //bitfield byte #3
    std::array<char, StringLength::CLIENT_ID_ATTR> clientIdAttr;
    std::optional<uint64_t> equityTradePrice;
    std::optional<uint32_t> equityTradeSize;
    std::optional<EquityTradeVenue> equityTradeVenue;
    std::optional<uint64_t> equityTransactTime;
    std::array<char, StringLength::EQUITY_BUY_CLEARING_FIRM> equityBuyClearingFirm;
    std::array<char, StringLength::EQUITY_SELL_CLEARING_FIRM> equitySellClearingFirm;
    std::optional<SessionEligibility> sessionEligibility;

    //bitfield byte #4
    std::optional<Compression> compression;
    std::array<char, StringLength::ORS> ors;
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

    uint8_t getMessageType() {return messageType;}

    uint8_t getMatchingUnit() {return matchingUnit;}

    uint16_t getMessageLength() {return this->messageLength;}
    void setMessageLength(const uint16_t messageLength) {this->messageLength = messageLength;}

    uint32_t getSequenceNumber() {return this->sequenceNumber;}
    void setSequenceNumber(const uint16_t sequenceNumber) {this->sequenceNumber = sequenceNumber;}

    std::string getCrossId() {return std::string(crossId.data(), strnlen(crossId.data(), crossId.size()));}
    void setCrossId(const std::array<char, StringLength::CROSS_ID>& crossId) {this->crossId = crossId;}

    std::array<char, StringLength::CROSS_TYPE>& getCrossType() {return this->crossType;}
    void setCrossType(const std::array<char, StringLength::CROSS_TYPE>& crossType) {this->crossType = crossType;}

    std::array<char, StringLength::CROSS_PRIORITIZATION>& getCrossPrioritization() {return this->crossPrioritization;}
    void setCrossPrioritization(const std::array<char, StringLength::CROSS_PRIORITIZATION>& crossPrioritization) {this->crossPrioritization = crossPrioritization;}

    uint64_t getPrice() {return this->price;}
    void setPrice(const uint64_t price) {this->price = price;}

    uint32_t getOrderQuantity() {return this->orderQuantity;}
    void setOrderQuantity(const uint32_t orderQuantity) {this->orderQuantity = orderQuantity;}

    uint8_t getNumberOfNewOrderCrossBitfields() {return this->numberOfNewOrderCrossBitfields;}
    void setNumberOfNewOrderCrossBitfields(const uint8_t numberOfNewOrderCrossBitfields) {this->numberOfNewOrderCrossBitfields = numberOfNewOrderCrossBitfields;}

    std::vector<NewOrderCrossBitfield>& getNewOrderCrossBitfields() {return this->newOrderCrossBitfields;}
    void setNewOrderCrossBitfields (const std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields) {this->newOrderCrossBitfields = newOrderCrossBitfields;}

    uint16_t getGroupCount() {return this->groupCount;}
    void setGroupCount(const uint16_t groupCount) {this->groupCount = groupCount;}

    std::vector<RepeatingGroup>& getRepeatingGroups() {return this->repeatingGroups;}
    void setRepeatingGroups(const std::vector<RepeatingGroup>& repeatingGroups) {this->repeatingGroups = repeatingGroups;}

    NonRepeatingOptionalField getNonRepeatingOptionalFields() {return this->nonRepeatingOptionalFields;}
    void setNonRepeatingOptionalField(const NonRepeatingOptionalField nonRepeatingOptionalField) {this->nonRepeatingOptionalFields = nonRepeatingOptionalField;}

private:
    //helper varibales
    const std::string stringPayload;
    size_t numberOfFieldsInRepeatingFields;

public:
    //helper functions
    size_t getNumberOfFieldsInRepeatingFields() {return this->numberOfFieldsInRepeatingFields;}
    void setNumberOfFieldsInRepeatingFields(const size_t numberOfFieldsInRepeatingFields) {this->numberOfFieldsInRepeatingFields = numberOfFieldsInRepeatingFields;}

};

inline void to_json(nlohmann::ordered_json& j, const NewOrderCrossBitfield& b) {
    j = nlohmann::json{{"bitfield", b.bitfield}};
}

inline void to_json(nlohmann::ordered_json& j, const RepeatingOptionalBitfield& r) {
    j = nlohmann::ordered_json{
        {"account", std::string(r.account.data(), strnlen(r.account.data(), r.account.size()))},
        {"clearingOptionalData", std::string(r.clearingOptionalData.data(), strnlen(r.clearingOptionalData.data(), r.clearingOptionalData.size()))},
        {"frequentTraderId", std::string(r.frequentTraderId.data(), strnlen(r.frequentTraderId.data(), r.frequentTraderId.size()))}
    };
    if (r.cmtaNumber) j["cmtaNumber"] = *r.cmtaNumber;
    if (r.clearingAccount) j["clearingAccount"] = *r.clearingAccount;
}

inline void to_json(nlohmann::ordered_json& j, const RepeatingGroup& r) {
    j = nlohmann::ordered_json{
        {"side", r.side},
        {"allocQuantity", r.allocQuantity},
        {"clientOrderId", std::string(r.clientOrderId.data(), strnlen(r.clientOrderId.data(), r.clientOrderId.size()))},
        {"capacity", r.capacity},
        {"openClose", r.openClose},
        {"giveUpFirmId", std::string(r.giveUpFirmId.data(), strnlen(r.giveUpFirmId.data(), r.giveUpFirmId.size()))},
        {"repeatingOptionalBitfields", r.repeatingOptionalBitfields}
    };
}

inline void to_json(nlohmann::ordered_json& j, const Side& s) {
    switch(s) {
        case Side::BUY: j = "BUY"; break;
        case Side::SELL: j = "SELL"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const Capacity& c) {
    switch(c) {
        case Capacity::CUSTOMER:j = "CUSTOMER";break;
        case Capacity::MARKET_MAKER:  j = "MARKET_MAKER"; break;
        case Capacity::FIRM: j = "FIRM";  break;
        case Capacity::PROFESSIONAL_CUSTOMER:j = "PROFESSIONAL_CUSTOMER";break;
        case Capacity::AWAYMARKET_MAKER: j = "AWAYMARKET_MAKER";  break;
        case Capacity::BROKER_DEALER: j = "BROKER_DEALER";break;
        case Capacity::JOINT_BACK_OFFICE: j = "JOINT_BACK_OFFICE"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const OpenClose& o) {
    switch(o) {
        case OpenClose::OPEN: j = "OPEN"; break;
        case OpenClose::CLOSE: j = "CLOSE"; break;
        case OpenClose::NONE: j = "NONE";  break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const NonRepeatingOptionalField& f) {
    auto addIfValid = [&](const char* data, size_t size, const std::string& key) {
        std::string str(data, strnlen(data, size));
        // Check every character is printable ASCII
        bool valid = !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return c >= 32 && c < 127;
        });
        if (valid) j[key] = str;
    };

    addIfValid(f.symbol.data(), f.symbol.size(), "symbol");
    addIfValid(f.targetPartyId.data(), f.targetPartyId.size(), "targetPartyId");
    addIfValid(f.preventMatch.data(), f.preventMatch.size(), "preventMatch");
    addIfValid(f.routingFirmId.data(), f.routingFirmId.size(), "routingFirmId");
    addIfValid(f.clientIdAttr.data(), f.clientIdAttr.size(), "clientIdAttr");
    addIfValid(f.equityBuyClearingFirm.data(), f.equityBuyClearingFirm.size(), "equityBuyClearingFirm");
    addIfValid(f.equitySellClearingFirm.data(), f.equitySellClearingFirm.size(), "equitySellClearingFirm");
    addIfValid(f.ors.data(), f.ors.size(), "ors");

    if (f.maturityDate) j["maturityDate"] = *f.maturityDate;
    if (f.strikePrice) j["strikePrice"] = *f.strikePrice;
    if (f.putOrCall) j["putOrCall"] = *f.putOrCall;
    if (f.execInst) j["execInst"] = *f.execInst;
    if (f.attributedQuote) j["attributedQuote"] = *f.attributedQuote;
    if (f.autoMatch) j["autoMatch"] = *f.autoMatch;
    if (f.autoMatchPrice) j["autoMatchPrice"] = *f.autoMatchPrice;
    if (f.lastPriority) j["lastPriority"] = *f.lastPriority;
    if (f.equityTradePrice) j["equityTradePrice"] = *f.equityTradePrice;
    if (f.equityTradeSize) j["equityTradeSize"] = *f.equityTradeSize;
    if (f.equityTradeVenue) j["equityTradeVenue"] = *f.equityTradeVenue;
    if (f.equityTransactTime) j["equityTransactTime"] = *f.equityTransactTime;
    if (f.sessionEligibility) j["sessionEligibility"] = *f.sessionEligibility;
    if (f.compression) j["compression"] = *f.compression;
}

inline void to_json(nlohmann::ordered_json& j, const PutOrCall& p) {
    switch(p) {
        case PutOrCall::PUT: j = "PUT"; break;
        case PutOrCall::CALL: j = "CALL"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const AttributedQuote& a) {
    switch(a) {
        case AttributedQuote::NO_FIRM_ID: j = "NO_FIRM_ID"; break;
        case AttributedQuote::ATTRIBUTE_FIRM_ID: j = "ATTRIBUTE_FIRM_ID"; break;
        case AttributedQuote::CLIENT_ID_ONLY: j = "CLIENT_ID_ONLY"; break;
        case AttributedQuote::CLEARING_CLIENT: j = "CLEARING_CLIENT"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const AutoMatch& a) {
    switch(a) {
        case AutoMatch::DISABLED: j = "DISABLED"; break;
        case AutoMatch::MARKET: j = "MARKET"; break;
        case AutoMatch::LIMIT: j = "LIMIT"; break;
        case AutoMatch::MARKET_NA: j = "MARKET_NA"; break;
        case AutoMatch::LIMIT_NA: j = "LIMIT_NA"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const LastPriority& l) {
    switch(l) {
        case LastPriority::DISABLED: j = "DISABLED"; break;
        case LastPriority::ENABLED:  j = "ENABLED"; break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const EquityTradeVenue& e) {
    switch(e) {
        case EquityTradeVenue::NYSE_AMERICAN: j = "NYSE_AMERICAN"; break;
        case EquityTradeVenue::NASDAQ_TEXAS:j = "NASDAQ_TEXAS"; break;
        case EquityTradeVenue::NYSE_NATIONAL: j = "NYSE_NATIONAL"; break;
        case EquityTradeVenue::INVESTORS_EXCHANGE:j = "INVESTORS_EXCHANGE";break;
        case EquityTradeVenue::CBOE_EDGA_EXCHANGE:j = "CBOE_EDGA_EXCHANGE";break;
        case EquityTradeVenue::CBOE_EDGX_EXCHANGE:j = "CBOE_EDGX_EXCHANGE";break;
        case EquityTradeVenue::CHX: j = "CHX"; break;
        case EquityTradeVenue::NYSE:  j = "NYSE"; break;
        case EquityTradeVenue::NYSE_ARCO: j = "NYSE_ARCO"; break;
        case EquityTradeVenue::NASDAQ:  j = "NASDAQ"; break;
        case EquityTradeVenue::NASDAQ_PSX: j = "NASDAQ_PSX"; break;
        case EquityTradeVenue::CBOE_BYX_EXCHANGE:j = "CBOE_BYX_EXCHANGE";break;
        case EquityTradeVenue::CBOE_BZX_EXCHANGE:j = "CBOE_BZX_EXCHANGE";break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const SessionEligibility& s) {
    switch(s) {
        case SessionEligibility::REGULAR:  j = "REGULAR"; break;
        case SessionEligibility::GLOBAL_REGULAR:  j = "GLOBAL_REGULAR"; break;
        case SessionEligibility::RTH_CURB_SESSION:j = "RTH_CURB_SESSION";break;
    }
}

inline void to_json(nlohmann::ordered_json& j, const Compression& c) {
    switch(c) {
        case Compression::NO:  j = "NO"; break;
        case Compression::YES: j = "YES"; break;
    }
}

