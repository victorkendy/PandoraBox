

#include <gtest/gtest.h>

#include "pbge/utils/converters.h"

TEST(NumberConversion, shouldParseInts) {
    EXPECT_EQ(159, pbge::stringToNumber<int>("159"));
}

TEST(NumberConversion, shouldParseFloatValues) {
    EXPECT_EQ(159.23f, pbge::stringToNumber<float>("159.23"));
}

TEST(NumberConversion, shouldParseDoubleValues) {
    EXPECT_EQ(159.23, pbge::stringToNumber<double>("159.23"));
}
