#pragma once
#include <string>
#include "../Messages/NewOrderCrossMsg.h"


class Decoder {
public:
    Decoder(const std::string &hexPayload);
    void initiateDecoder();
    //uint16_t hexLittleToUint16(const std::string &hex); **testing

private:
    std::string payload;
    size_t currOffset;
    NewOrderCrossMessageFields newOrderCrossMessageFields;

    void handleStartOfMessage(); //rows 1-10 **beginning message format
    void handleNumberOfNewOrderBitfields(); //rows 11-12 (really n) **bitfields format
    void handleRepeatingGroups(); //13-18 **repeating groups mandatory format
    void handleRepeatingOptionalGroups(); //19-23 **repeating optional groups
    void handleNonRepeatingOptionalGroups(); //24 **non-repeating optional groups

    uint16_t hexLittleToUint16(const std::string &hex);

};