#include <iostream>
#include <fstream>
#include <string>
#include "../src/NewOrderCrossMsg.h"

void readHexPayload(std::string hexPayload) {
    std::string filePath = std::string("../tmp/") + hexPayload;
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
}

int main(int argc, char* arg[]) {
    if(argc < 2) {
        std::cout << "Failed to run program" << '\n';
        return 1;
    }
    readHexPayload(arg[1]);
    return 0;
}
