# NOC Engine 2.11.101

This is a Low Latency new order cross parser for a Binary Order Entry Protocol Engine inspired from CBOE's 2.11.101 specifications. The purpose of this program is to let two parties trade simultaneously exchange stocks or in other words the broker and firm represent the buyer and the seller. NOC is great because it can avoid unnecessary costs, both parties agree on price upfront, and promotes fairness. Apart from its counterpart the FIX8 Engine, the binary protocol is mainly used in High Frequency Trading firms who speed over uniformity or are exchange specific to CBOE.

## Where I'm at in the Program

The first draft of the decoder is complete and it successfully parses raw BOE NewOrderCross messages including all mandatory fields, repeating groups, and optional bitfields. I'm going to have to optimize this ALOT because 11 microseconds is pretty mid. Also the way I did it was by converting the string message in Hex however it would be more traditional to just grab the data in hex and just use memcpy but I guess it's too late now. Maybe later down the road I will change it if I'm motivated. If I just used raw bytes the decode time would be around 1 microsecond but im dumb and made everything harder. Things to do next is probably format the NewOrderCrossMessageField data structure and organize it into JSON so users can see the order output which will not be included in decode time.

## Performance

Current decode time on a NewOrderCross message: 0.000011208 seconds.

## Example

### Input Payload
```
BABAB0004100640000004E5A315637424A5F4163636570744275790000003131204E0000000000006400000002413003003164000000514C37535A37435F6167656E63790000000000004343444546470000000000000000003228000000514C394B3855565F636F6E74726131000000000000464F41424344270200005758595A323C000000514C39543559445F636F6E74726132000000000000464F414243447B0000005758595A303051306B410000000043444546
```

### Parsed Output
```json
{
    "Header": {
        "Message Type": 65,
        "Matching Unit": 0
    },
    "Body": {
        "Sequence Number": 100,
        "Cross ID": "NZ1V7BJ_AcceptBuy",
        "Cross Type": [49],
        "Cross Prioritization": [49],
        "Price": 20000,
        "Order Quantity": 6553700,
        "Number of New Order Cross Bitfields": 2,
        "New Order Cross Bitfields": {
            "Bitfields": [
                { "bitfield": 65 },
                { "bitfield": 48 }
            ]
        },
        "Group Count": 3,
        "Repeating Groups": {
            "Groups": [
                {
                    "side": "BUY",
                    "allocQuantity": 1358975232,
                    "clientOrderId": "L7SZ7C_agency",
                    "capacity": "CUSTOMER",
                    "openClose": null,
                    "giveUpFirmId": "EFG",
                    "repeatingOptionalBitfields": {
                        "account": "",
                        "clearingOptionalData": "",
                        "frequentTraderId": "",
                        "cmtaNumber": 0,
                        "clearingAccount": 0
                    }
                },
                {
                    "side": "SELL",
                    "allocQuantity": 1358975232,
                    "clientOrderId": "L9K8UV_contra1",
                    "capacity": "FIRM",
                    "openClose": "OPEN",
                    "giveUpFirmId": "ABCD",
                    "repeatingOptionalBitfields": {
                        "account": "",
                        "clearingOptionalData": "",
                        "frequentTraderId": "",
                        "cmtaNumber": 2555943,
                        "clearingAccount": 1515674199
                    }
                },
                {
                    "side": "SELL",
                    "allocQuantity": 1358975232,
                    "clientOrderId": "L9T5YD_contra2",
                    "capacity": "FIRM",
                    "openClose": "OPEN",
                    "giveUpFirmId": "ABCD",
                    "repeatingOptionalBitfields": {
                        "account": "",
                        "clearingOptionalData": "",
                        "frequentTraderId": "",
                        "cmtaNumber": 8061051,
                        "clearingAccount": 1515674199
                    }
                }
            ]
        },
        "Non Repeating Optional Bitfields": {
            "Optionals": null
        }
    }
}
```

## Build

```bash
cmake -S . -B build
cmake --build build
./build/MyProject docs/hexpayload.txt
```

## Testing

```bash
./build/MyProjectTests
```

## Tech Stack

- C++17
- CMake
- Google Test