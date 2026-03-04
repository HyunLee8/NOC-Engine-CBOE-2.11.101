# NOC Engine 2.11.101

This is a Low Latency new order cross parser for a Binary Order Entry Protocol Engine inspired from CBOE's 2.11.101 specifications. The purpose of this program is to let two parties trade simulataneously exchange stocks or in other words the broker and firm represent the buyer and the seller. NOC is great because it can avoid uncessary costs, both parties agree on price upfront, and promotes fairness. Apart from its counter part the FIX8 Engine, the binary protocol is mainly used in High Frequency Trading firms who speed over uniformity or are exchange specific to CBOE.

## Where I'm at in the Program

The first draft of the decoder is complete and it successfully parses raw BOE NewOrderCross messages including all mandatory fields, repeating groups, and optional bitfields. I'm going to have to optimize this ALOT because 11 microseconds is pretty mid. Also the way I did it was by converting the string message in Hex however it would be more traditional to just grab the data in hex and just use memcpy but I guess it's too late now. Maybe later down the road I will chage it if I'm motivated. If i just used raw bytes the decode time would be around 1 microsecond but im dumb and made everything harder. Things to do next is probably format the NewOrderCrossMessageField datastructure and organize it into JSON so users can see the order output which will not be included in decode time. 
r utility for benchmarking decode performance

## Performance

Current decode time on a NewOrderCross message: 0.000011208 seconds.

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