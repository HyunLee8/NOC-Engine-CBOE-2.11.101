#pragma once
#include <string>
#include "../Messages/NewOrderCrossMsg.h"


class Decoder {
public:
    Decoder(const std::string &hexPayload);

    void initiateDecoder();

    //uint16_t hexLittleToUint16(const std::string &hex); *uncomment for testing
    //template<size_t N>
    //void hexToChars(const std::string& hex, std::array<char, N>& arr) {
    //    for (size_t i = 0; i < N; i++) {
    //        std::string byte = hex.substr(i * 2, 2);
    //       arr[i] = static_cast<char>(std::stoul(byte, nullptr, 16));
    //   }
    //}

private:
    std::string payload;
    size_t currIndex;
    size_t currOffset;
    NewOrderCrossMessageFields newOrderCrossMessageFields;

    void handleStartOfMessage(); //rows 1-10 **beginning message format
    void handleNumberOfNewOrderBitfields(); //rows 11-12 (really n) **bitfields format
    void handleRepeatingGroups(); //13-18 **repeating groups mandatory format
    void handleRepeatingOptionalGroups(); //19-23 **repeating optional groups
    void handleNonRepeatingOptionalGroups(); //24 **non-repeating optional groups

    uint8_t hexToUint8(const std::string &hex);
    uint16_t hexLittleToUint16(const std::string &hex);
    uint32_t hexLittleToUint32(const std::string &hex);
    uint64_t hexLittleToUint64(const std::string &hex);


    template<size_t N>
    std::array<char, N> hexToChars(std::string_view hex) {
        // ← string_view instead of string
        std::array<char, N> arr;
        for (size_t i = 0; i < N; i++) {
            std::string byte(hex.substr(i * 2, 2)); // small temporary
            arr[i] = static_cast<char>(std::stoul(byte, nullptr, 16));
        }
        return arr;
    }


};
