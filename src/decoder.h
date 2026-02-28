#pragma once
#include <string>

class Decoder {
public:
    Decoder(const std::string &hexPayload);

    enum class Phase {
        START_OF_MESSAGE,
        MESSAGE_LENGTH,
        MESSAGE_TYPE,
        MATCHING_UNIT,
        SEQUENCE_NUMBER,
        CROSS_ID,
        CROSS_TYPE,
        CROSS_PRIORITIZATION,

    };

    void setPhase(Phase p);

    Phase getPhase() const;
    std::string getPayload();

    void charToHex();
    void stringToHex();
    void intToHex();
    
    void initiateDecoder(std::string payload);

private:
    std::string payload;
    Phase phase;
};