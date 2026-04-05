#include <vector>
#include <array>
#include <iomanip>
#include <string>
#include <string_view>
#include "Decoder/Decoder.h"
#include "Utils/HexConversion.h"

Decoder::Decoder(const std::string &hexPayload)
    : payload(hexPayload), currOffset(0), currIndex(0), newOrderCrossMessageFields(hexPayload){}

void Decoder::initiateDecoder() {
    handleStartOfMessage();
    handleNumberOfNewOrderBitfields();
    handleRepeatingGroups();
    handleNonRepeatingOptionalGroups();
}

void Decoder::handleStartOfMessage() {

    const std::string_view sv(payload);

    if (const uint16_t startOfMessage = hexLittleToUint16(std::string(sv.substr(0, 4))); startOfMessage != 47802) {
        throw std::invalid_argument("Invalid start of message");
    }
    const uint16_t messageLength = hexLittleToUint16(std::string(sv.substr(4, 4)));
    newOrderCrossMessageFields.setMessageLength(messageLength);

    const uint32_t sequenceNumber = hexLittleToUint32(std::string(sv.substr(12, 8)));
    newOrderCrossMessageFields.setSequenceNumber(sequenceNumber);

    newOrderCrossMessageFields.setCrossId(hexToChars<20>(sv.substr(20, 40)));

    newOrderCrossMessageFields.setCrossType(hexToChars<1>(sv.substr(60, 2)));

    newOrderCrossMessageFields.setCrossPrioritization(hexToChars<1>(sv.substr(62, 2)));

    const uint64_t price = hexLittleToUint64(std::string(sv.substr(64, 16)));
    newOrderCrossMessageFields.setPrice(price);

    const uint32_t orderQuantity = hexLittleToUint32(std::string(sv.substr(80, 8)));
    newOrderCrossMessageFields.setOrderQuantity(orderQuantity);

    currIndex += 88;
}

void Decoder::handleNumberOfNewOrderBitfields() {
    const std::string_view sv(payload);

    const uint8_t numberOfNewOrderCrossBitfields = hexToUint8 (std::string(sv.substr(88, 2)));
    newOrderCrossMessageFields.setNumberOfNewOrderCrossBitfields (numberOfNewOrderCrossBitfields);
    currIndex += 2;

    std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields = newOrderCrossMessageFields.getNewOrderCrossBitfields();
    for (int i{}; i < numberOfNewOrderCrossBitfields; ++i) {
        const uint8_t bitfield = hexToUint8(std::string(sv.substr(currIndex, 2)));
        newOrderCrossBitfields.push_back ({bitfield});
        currIndex += 2;
    }
}

void Decoder::handleRepeatingGroups() { //prepare for hell
    const std::string_view sv(payload);

    const uint16_t groupLength = hexLittleToUint16(std::string(sv.substr(currIndex, 4)));
    newOrderCrossMessageFields.setGroupCount(groupLength);
    currIndex += 4;

    const std::vector<NewOrderCrossBitfield>& newOrderCrossBitfields = newOrderCrossMessageFields.getNewOrderCrossBitfields();

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
        } else if (i == 1) {
            for (const auto& bitfield : BitfieldIndex2Values::values) {
                if (includedOptionalBitfields.at(i) & static_cast<uint8_t>(bitfield)) {
                    matchedBitfields2.push_back(static_cast<uint8_t>(bitfield));
                }
            }
        } else if (i == 2) {
            for (const auto& bitfield : BitfieldIndex3Values::values) {
                if (includedOptionalBitfields.at(i) & static_cast<uint8_t>(bitfield)) {
                    matchedBitfields3.push_back(static_cast<uint8_t>(bitfield));
                }
            }
        } else {
            for (const auto& bitfield : BitfieldIndex4Values::values) {
                if (includedOptionalBitfields.at(i) & static_cast<uint8_t>(bitfield)) {
                    matchedBitfields4.push_back(static_cast<uint8_t>(bitfield));
                }
            }
        }
    }

    std::vector<RepeatingGroup> repeatingGroups;
    repeatingGroups.resize(groupLength);
    for (int i{}; i < groupLength; ++i) {
        char side = hexToChar(std::string(sv.substr(currIndex, 2)));
        repeatingGroups.at(i).side = static_cast<Side>(side);
        currIndex += 4;

        const uint32_t allocQuantity = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
        repeatingGroups.at(i).allocQuantity = allocQuantity;
        currIndex += 8;

        const std::array<char, StringLength::CLIENT_ORDER_ID> clientOrderId = hexToChars<StringLength::CLIENT_ORDER_ID>(std::string(sv.substr(currIndex, StringLength::CLIENT_ORDER_ID*2)));
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
                default: ;
            }
        }

        //need this in seperate loop because it's Byte 4 index
        for (const auto& bitfield : matchedBitfields4) {
            if (bitfield == static_cast<uint8_t>(BitfieldIndex4::FREQUENT_TRADER_ID)) {
                const auto frequentTraderId = hexToChars<StringLength::FREQUENT_TRADER_ID>(std::string(sv.substr(currIndex, StringLength::FREQUENT_TRADER_ID*2)));
                repeatingGroups.at(i).repeatingOptionalBitfields.frequentTraderId = frequentTraderId;
                currIndex += (StringLength::FREQUENT_TRADER_ID*2);
                break;
            }
        }
    }
    //sets entire vector we made to that
    newOrderCrossMessageFields.setRepeatingGroups(repeatingGroups);
}

void Decoder::handleNonRepeatingOptionalGroups() {
    NonRepeatingOptionalField nonRepeatingOptionalFields;

    const std::string_view sv(payload);
    for (const auto& bitfield : matchedBitfields1) {
        switch (bitfield) {
            case static_cast<uint8_t>(BitfieldIndex1::SYMBOL): {
                std::array<char, StringLength::SYMBOL> symbol = hexToChars<StringLength::SYMBOL>(std::string(sv.substr(currIndex, StringLength::SYMBOL*2)));
                nonRepeatingOptionalFields.symbol = symbol;
                currIndex += (StringLength::SYMBOL*2);
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::MATURITY_DATE): {
                uint32_t maturityDate = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
                nonRepeatingOptionalFields.maturityDate = maturityDate;
                currIndex += 8;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::STRIKE_PRICE): {
                uint64_t strikePrice = hexLittleToUint64(std::string(sv.substr(currIndex, 16)));
                nonRepeatingOptionalFields.strikePrice = strikePrice;
                currIndex += 16;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::PUT_OR_CALL): {
                char putOrCall = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.putOrCall = static_cast<PutOrCall>(putOrCall);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::EXEC_INSTRUCTION): {
                char execInstruction = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.execInst = execInstruction;
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::ATTRIBUTED_QUOTE): {
                char attributedQuote = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.attributedQuote = static_cast<AttributedQuote>(attributedQuote);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::TARGET_PARTY_ID): {
                std::array<char, StringLength::TARGET_PARTY_ID> targetPartyId = hexToChars<StringLength::TARGET_PARTY_ID>(std::string(sv.substr(currIndex, StringLength::TARGET_PARTY_ID*2)));
                nonRepeatingOptionalFields.targetPartyId = targetPartyId;
                currIndex += (StringLength::TARGET_PARTY_ID*2);
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex1::PREVENT_MATCH): {
                std::array<char, StringLength::PREVENT_MATCH> preventMatch = hexToChars<StringLength::PREVENT_MATCH>(std::string(sv.substr(currIndex, StringLength::PREVENT_MATCH*2)));
                nonRepeatingOptionalFields.preventMatch = preventMatch;
                currIndex += (StringLength::PREVENT_MATCH*2);
                break;
            }
            default: ;
        }
    }

    for (const auto& bitfield : matchedBitfields2) {
        switch (bitfield) {
            case static_cast<uint8_t>(BitfieldIndex2::AUTO_MATCH): {
                char autoMatch = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.autoMatch = static_cast<AutoMatch>(autoMatch);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex2::AUTO_MATCH_PRICE): {
                uint64_t autoMatchPrice = hexLittleToUint64(std::string(sv.substr(currIndex, 16)));
                nonRepeatingOptionalFields.autoMatchPrice = autoMatchPrice;
                currIndex += 16;
            }
            case static_cast<uint8_t>(BitfieldIndex2::LAST_PRIORITY): {
                char lastPriority = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.lastPriority = static_cast<LastPriority>(lastPriority);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex2::ROUTING_FIRM_ID): {
                std::array<char, StringLength::ROUTING_FIRM_ID> routingFirmId = hexToChars<StringLength::ROUTING_FIRM_ID>(std::string(sv.substr(currIndex, StringLength::ROUTING_FIRM_ID*2)));
                nonRepeatingOptionalFields.routingFirmId = routingFirmId;
                currIndex += (StringLength::ROUTING_FIRM_ID*2);
                break;
            }
            default: ;
        }
    }

    for (const auto& bitfield : matchedBitfields3) {
        switch (bitfield) {
            case static_cast<uint8_t>(BitfieldIndex3::CLIENT_ID_ATTR): {
                const auto clientIdAttr = hexToChars<StringLength::CLIENT_ID_ATTR>(std::string(sv.substr(currIndex, StringLength::CLIENT_ID_ATTR*2)));
                nonRepeatingOptionalFields.clientIdAttr = clientIdAttr;
                currIndex += (StringLength::CLIENT_ID_ATTR*2);
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_TRADE_PRICE): {
                uint64_t equityTradePrice = hexLittleToUint64(std::string(sv.substr(currIndex, 16)));
                nonRepeatingOptionalFields.equityTradePrice = equityTradePrice;
                currIndex += 16;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_TRADE_SIZE): {
                uint32_t equityTradeSize = hexLittleToUint32(std::string(sv.substr(currIndex, 8)));
                nonRepeatingOptionalFields.equityTradeSize = equityTradeSize;
                currIndex += 8;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_TRADE_VENUE): {
                char equityTradeVenue = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.equityTradeVenue = static_cast<EquityTradeVenue>(equityTradeVenue);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_TRANSACT_TIME): {
                uint64_t equityTransactTime = hexLittleToUint64(std::string(sv.substr(currIndex, 16)));
                nonRepeatingOptionalFields.equityTransactTime = equityTransactTime;
                currIndex += 16;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_BUY_CLEARING_FIRM): {
                const auto buyClearingFirm = hexToChars<StringLength::EQUITY_BUY_CLEARING_FIRM>(std::string(sv.substr(currIndex, StringLength::EQUITY_BUY_CLEARING_FIRM*2)));
                nonRepeatingOptionalFields.equityBuyClearingFirm = buyClearingFirm;
                currIndex += (StringLength::EQUITY_BUY_CLEARING_FIRM*2);
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::EQUITY_SELL_CLEARING_FIRM): {
                const auto sellClearingFirm = hexToChars<StringLength::EQUITY_SELL_CLEARING_FIRM>(std::string(sv.substr(currIndex, StringLength::EQUITY_SELL_CLEARING_FIRM*2)));
                nonRepeatingOptionalFields.equitySellClearingFirm = sellClearingFirm;
                currIndex += (StringLength::EQUITY_SELL_CLEARING_FIRM*2);
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex3::SESSION_ELIGIBILITY): {
                char sessionEligibility = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.sessionEligibility = static_cast<SessionEligibility>(sessionEligibility);
                currIndex += 2;
                break;
            }
            default: ;
        }
    }

    for (const auto& bitfield : matchedBitfields4) {
        switch (bitfield) {
            case static_cast<uint8_t>(BitfieldIndex4::COMPRESSION): {
                char compression = hexToChar(std::string(sv.substr(currIndex, 2)));
                nonRepeatingOptionalFields.compression = static_cast<Compression>(compression);
                currIndex += 2;
                break;
            }
            case static_cast<uint8_t>(BitfieldIndex4::ORS): {
                std::array<char, StringLength::ORS> ors = hexToChars<StringLength::ORS>(std::string(sv.substr(currIndex, StringLength::ORS*2)));
                nonRepeatingOptionalFields.ors = ors;
                currIndex += (StringLength::ORS*2);
                break;
            }
            default: ;
        }
    }
}