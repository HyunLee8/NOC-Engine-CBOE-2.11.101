#include "Serialization.h"
#include <nlohmann/json.hpp>
#include <iostream>

Serializer::Serializer(NewOrderCrossMessageFields& newOrderCrossMessage)
    : sortedFields(newOrderCrossMessage) {}

void Serializer::parseMessage() {
    output["Header"]["Message Type"] = sortedFields.getMessageType();
    output["Header"]["Matching Unit"] = sortedFields.getMatchingUnit();

    output["Body"]["Sequence Number"] = sortedFields.getSequenceNumber();
    output["Body"]["Cross ID"] = sortedFields.getCrossId();
    output["Body"]["Cross Type"] = sortedFields.getCrossType();
    output["Body"]["Cross Prioritization"] = sortedFields.getCrossPrioritization();
    output["Body"]["Price"] = sortedFields.getPrice();
    output["Body"]["Order Quantity"] = sortedFields.getOrderQuantity();
    output["Body"]["Number of New Order Cross Bitfields"] = sortedFields.getNumberOfNewOrderCrossBitfields();

    for (const auto& bitfield : sortedFields.getNewOrderCrossBitfields()) {
        output["Body"]["New Order Cross Bitfields"]["Bitfields"].push_back(bitfield);
    }

    output["Body"]["Group Count"] = sortedFields.getGroupCount();

    for (const auto& group : sortedFields.getRepeatingGroups()) {
        output["Body"]["Repeating Groups"]["Groups"].push_back(group);
    }

    output["Body"]["Non Repeating Optional Bitfields"]["Optionals"] = sortedFields.getNonRepeatingOptionalFields();
}

void Serializer::displayMessage() {
    std::cout << output.dump(4) << "\n";
}
