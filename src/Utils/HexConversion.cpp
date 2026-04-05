#include <string>
#include <string_view>

uint8_t hexToUint8(const std::string& hex) {
    return static_cast<uint8_t>(std::stoull(hex, nullptr, 16));
}

uint16_t hexLittleToUint16(const std::string_view& hex) {
    const uint16_t temp = static_cast<uint16_t>(std::stoul(hex, nullptr, 16));
    return (temp >> 8) | (temp << 8);
}

uint32_t hexLittleToUint32(const std::string_view& hex) {
    const uint32_t temp = static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
    return (temp >> 24) | (temp << 24) | ((temp << 8) & 0xFF00) | ((temp >> 8) & 0xFF0000);
}

uint64_t hexLittleToUint64(const std::string_view& hex) {
    const uint64_t temp = static_cast<uint64_t>(std::stoull(hex, nullptr, 16));
    return (temp >> 56) | ((temp >> 40) & 0xFF00) | ((temp >> 24) & 0xFF0000) |
           ((temp >> 8) & 0xFF000000) | ((temp << 8) & 0xFF00000000) |
           ((temp << 24) & 0xFF0000000000) | ((temp << 40) & 0xFF000000000000) | (temp << 56);
}

char hexToChar(std::string_view hex) {
    return static_cast<char>(std::stoul(std::string(hex), nullptr, 16));
}