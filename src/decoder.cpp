#include <iostream>
#include <vector>
#include <array>
#include "decoder.h"

Decoder::Decoder(const std::string &hexPayload)
    : payload(hexPayload), phase(Phase::START_OF_MESSAGE){}

