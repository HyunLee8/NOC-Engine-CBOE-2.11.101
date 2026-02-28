#include <iostream>
#include <fstream>
#include <string>
#include "../src/Decoder/Decoder.h"
#include "../src/Utils/Timer.h"

std::string readHexPayload(const std::string &hexPayload) {
    std::string filePath = std::string("../docs/") + hexPayload;
    std::ifstream file(filePath);

    if(!file.is_open()) {
        std::cout << "Failed to open file" << '\n';
    }

    std::string str;
    std::string fileContent;
    while(std::getline(file, str)) {
        fileContent += str;
        fileContent.push_back('\n');
    }

    std::cout << fileContent << '\n';

    return fileContent;
}

void runProcess(Decoder& decoder) {
    Timer timer;

    timer.start();
    decoder.initiateDecoder();
    timer.stop();
    double decodingTime = timer.getTime();

    //timer.start();
    //encoder.initiateEncoder();
    //timer.stop();
    //double encodingTime = timer.getTime();

}

void displayOrder() {

}



int main(int argc, char* arg[]) {
    if(argc < 2) {
        std::cout << "Failed to run program" << '\n';
        return 1;
    }

    std::string payload = readHexPayload(arg[1]);
    Decoder decoder(payload);
    runProcess(decoder);
    displayOrder();

    return 0;
}
