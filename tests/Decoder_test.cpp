#include <gtest/gtest.h>
#include "../src/Decoder/decoder.h"

class DecoderTest : public ::testing::Test {
protected:
    Decoder decoder{"test pay laod"};
};

TEST_F(DecoderTest, conversion) {
    EXPECT_EQ(decoder.hexLittleToUint16("0100"), 1);
}

TEST_F(DecoderTest, anotherTest) {
    EXPECT_EQ(decoder.hexLittleToUint16("0010"), 4096);
}