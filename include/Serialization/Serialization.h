#include <nlohmann/json_fwd.hpp>
#include "Messages/NewOrderCrossMsg.h"

class Serializer {
private:
    NewOrderCrossMessageFields sortedFields;
    nlohmann::ordered_json output;

public:
    Serializer(NewOrderCrossMessageFields& newOrderCrossMessageFields);
    void parseMessage();
    void displayMessage();
};