#include <iostream>
#include <fstream>
#include <string>
#include "../src/Decoder/Decoder.h"
#include "../src/Utils/Timer.h"

std::string readHexPayload(const std::string &hexPayload) {
    std::ifstream file(hexPayload);

    if(!file.is_open()) {
        std::cout << "Failed to open file" << '\n';
    }

    std::string str;
    std::string fileContent;
    while(std::getline(file, str)) {
        fileContent += str;
    }

    std::cout << fileContent << '\n';

    return fileContent;
}

void runProcess(Decoder& decoder) {
    Timer timer;

    timer.start();
    decoder.initiateDecoder();
    timer.stop();
    std::cout << "Decoding Time: " <<std::fixed << std::setprecision(9) << timer.getTimeSeconds() << " seconds\n";


    //timer.start();
    //encoder.initiateEncoder();
    //timer.stop();
    //double encodingTime = timer.getTime();

}



int main(int argc, char* arg[]) {
    if(argc < 2) {
        std::cout << "Failed to run program" << '\n';
        return 1;
    }

    std::string payload = readHexPayload(arg[1]);
    Decoder decoder(payload);
    runProcess(decoder);

    return 0;
}
