#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include "Decoder/Decoder.h"
#include "Utils/Timer.h"

std::string readHexPayload(const std::filesystem::path& payloadPath) {
    std::ifstream file(payloadPath);

    if(!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::string str;
    std::string fileContent;
    while(std::getline(file, str)) {
        fileContent += str;
    }

    std::cout << fileContent;

    return fileContent;
}

void runProcess(Decoder& decoder) {
    Timer timer;

    timer.start();
    decoder.initiateDecoder();
    timer.stop();

    std::cout << "\nDecoding Time: " <<std::fixed << std::setprecision(9) << timer.getTimeSeconds() << " seconds\n";
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            throw std::runtime_error("Not enough arguments");
        }

        std::filesystem::path path = argv[1];

        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("File not found");
        }

        std::string payload = readHexPayload(path);
        Decoder decoder(payload);
        runProcess(decoder);

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
