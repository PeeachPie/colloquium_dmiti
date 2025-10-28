#include <gtest/gtest.h>

#include "integer.hpp"

TEST(IntegerTests, DefaultIsZero) {
    Integer z;
    EXPECT_EQ(z.as_string(), "0");
    EXPECT_EQ(z.POZ_Z_D(), 0);
}

TEST(IntegerTests, ParsePositiveWithLeadingZeros) {
    Integer x("000123");
    EXPECT_EQ(x.as_string(), "123");
    EXPECT_EQ(x.POZ_Z_D(), 2);
}

TEST(IntegerTests, ParseNegativeWithLeadingZeros) {
    Integer x("-000123");
    EXPECT_EQ(x.as_string(), "-123");
    EXPECT_EQ(x.POZ_Z_D(), 1);
}

TEST(IntegerTests, ParseExplicitPlusZero) {
    Integer x("+0000");
    EXPECT_EQ(x.as_string(), "0");
    EXPECT_EQ(x.POZ_Z_D(), 0);
}

TEST(IntegerTests, InvalidInputs) {
    EXPECT_THROW(Integer("number"), std::invalid_argument);
    EXPECT_THROW(Integer("+"), std::invalid_argument);
    EXPECT_THROW(Integer("-"), std::invalid_argument);
}

TEST(IntegerTests, AbsReturnsNatural) {
    Integer x("-00123");
    Natural n = x.ABS_Z_N();
    EXPECT_EQ(n.as_string(), "123");
}

TEST(IntegerTests, NegationFlipsSignExceptZero) {
    Integer x("-5");
    x.MUL_ZM_Z();
    EXPECT_EQ(x.as_string(), "5");

    Integer z("0");
    z.MUL_ZM_Z();
    EXPECT_EQ(z.as_string(), "0");
}

TEST(IntegerTests, TransformationsBetweenZAndN) {
    Natural n("00012");
    Integer z = Integer().TRANS_N_Z(n);
    EXPECT_EQ(z.as_string(), "12");
    EXPECT_NO_THROW(z.TRANS_Z_N());
    EXPECT_EQ(z.TRANS_Z_N().as_string(), "12");

    Integer neg("-7");
    EXPECT_THROW(neg.TRANS_Z_N(), std::invalid_argument);
}

TEST(IntegerArithmetic, AddSub) {
    Integer a("123");
    Integer b("-50");
    EXPECT_EQ(a.ADD_ZZ_Z(b).as_string(), "73");
    EXPECT_EQ(a.SUB_ZZ_Z(b).as_string(), "173");
    EXPECT_EQ(Integer("-10").ADD_ZZ_Z(Integer("-5")).as_string(), "-15");
    EXPECT_EQ(Integer("-10").SUB_ZZ_Z(Integer("5")).as_string(), "-15");
    EXPECT_EQ(Integer("1000").SUB_ZZ_Z(Integer("1000")).as_string(), "0");
}

TEST(IntegerArithmetic, Mul) {
    EXPECT_EQ(Integer("0").MUL_ZZ_Z(Integer("123")).as_string(), "0");
    EXPECT_EQ(Integer("12").MUL_ZZ_Z(Integer("-3")).as_string(), "-36");
    EXPECT_EQ(Integer("-12").MUL_ZZ_Z(Integer("-3")).as_string(), "36");
}

TEST(IntegerArithmetic, DivMod) {
    EXPECT_EQ(Integer("10").DIV_ZZ_Z(Integer("3")).as_string(), "3");
    EXPECT_EQ(Integer("10").MOD_ZZ_Z(Integer("3")).as_string(), "1");

    EXPECT_EQ(Integer("-10").DIV_ZZ_Z(Integer("3")).as_string(), "-4");
    EXPECT_EQ(Integer("-10").MOD_ZZ_Z(Integer("3")).as_string(), "2");

    EXPECT_EQ(Integer("10").DIV_ZZ_Z(Integer("-3")).as_string(), "-4");
    EXPECT_EQ(Integer("10").MOD_ZZ_Z(Integer("-3")).as_string(), "2");

    EXPECT_EQ(Integer("-10").DIV_ZZ_Z(Integer("-3")).as_string(), "3");
    EXPECT_EQ(Integer("-10").MOD_ZZ_Z(Integer("-3")).as_string(), "1");

    EXPECT_THROW(Integer("5").DIV_ZZ_Z(Integer("0")), std::invalid_argument);
    EXPECT_THROW(Integer("5").MOD_ZZ_Z(Integer("0")), std::invalid_argument);
}

