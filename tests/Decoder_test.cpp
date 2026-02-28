#include <gtest/gtest.h>
#include "../src/Decoder/Decoder.h"

class DecoderTest : public ::testing::Test {
protected:
    Decoder decoder{"test pay laod"};
};

TEST_F(DecoderTest, conversion) {
    //EXPECT_EQ(decoder.hexLittleToUint16("0100"), 1);
}

TEST_F(DecoderTest, anotherTest) {
    //EXPECT_EQ(decoder.hexLittleToUint16("0010"), 4096);
}

TEST_F(DecoderTest, hexToChar) {
    std::array<char, 16> crossID;
    //decoder.hexToChars("4E5A31563742345F4163636570744275", crossID);

    EXPECT_EQ(crossID[0], 'N');
    EXPECT_EQ(crossID[1], 'Z');
    EXPECT_EQ(crossID[2], '1');
    // or check the whole array at once:
    EXPECT_EQ(std::string(crossID.data(), 16), "NZ1V7B4_AcceptBu");
}