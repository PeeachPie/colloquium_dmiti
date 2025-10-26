#include <gtest/gtest.h>
#include "natural.hpp"

TEST(NaturalTests, CreateDefault) {
    EXPECT_EQ(Natural().as_string(), "0");
}

TEST(NaturalTests, InvalidString) {
    EXPECT_THROW(Natural("number"), std::invalid_argument);
}