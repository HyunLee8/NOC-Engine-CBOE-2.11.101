#pragma once
#include <string_view>

uint8_t hexToUint8(std::string_view hex);
uint16_t hexLittleToUint16(std::string_view hex);
uint32_t hexLittleToUint32(std::string_view hex);
uint64_t hexLittleToUint64(std::string_view hex);
char hexToChar(std::string_view hex);

//Mostly because of optional field
//its easier to just use a template
//making for keeping uniformity since
//you're just passing in the string hex.

template<size_t N>
std::array<char, N> hexToChars(std::string_view hex) {
    std::array<char, N> arr;
    for (size_t i = 0; i < N; i++) {
        std::string byte(hex.substr(i * 2, 2)); // small temporary
        arr[i] = static_cast<char>(std::stoul(byte, nullptr, 16));
    }
    return arr;
}