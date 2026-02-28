#pragma once
#include <string>
#include <array>
#include <cstdint>

class Decoder {
public:
    Decoder(const std::string &hexPayload);

    void charToHex();
    void stringToHex();
    void intToHex();
    
    void initiateDecoder();

private:
    std::string payload;

    void handleStartOfMessage(); //rows 1-10 **beginning message format
    void handleNumberOfNewOrderBitfields(); //rows 11-12 (really n) **bitfields format
    void handleRepeatingGroups(); //13-18 **repeating groups mandatory format
    void handleRepeatingOptionalGroups(); //19-23 **repeating optional groups
    void handleNonRepeatingOptionalGroups(); //24 **non-repeating optional groups

};