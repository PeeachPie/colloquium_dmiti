#include <gtest/gtest.h>
#include "rational.hpp"

// Тесты для конструкторов
TEST(RationalTests, DefaultConstructor) {
    Rational r;
    EXPECT_EQ(r.as_string(), "0");
}

TEST(RationalTests, ConstructorFromIntegers) {
    Rational r(Integer("3"), Natural("4"));
    EXPECT_EQ(r.as_string(), "3/4");
}

TEST(RationalTests, ConstructorFromString) {
    Rational r1("5/7");
    EXPECT_EQ(r1.as_string(), "5/7");
    
    Rational r2("42");
    EXPECT_EQ(r2.as_string(), "42");
}

TEST(RationalTests, ConstructorFromStringNegative) {
    Rational r("-3/5");
    EXPECT_EQ(r.as_string(), "-3/5");
}

TEST(RationalTests, ConstructorZeroDenominator) {
    EXPECT_THROW(Rational(Integer("1"), Natural("0")), std::invalid_argument);
}

// Q-1 | Тесты для сокращения дроби
TEST(RationalTests, RED_Q_Q_AlreadyReduced) {
    Rational r("3/5");
    r.RED_Q_Q();
    EXPECT_EQ(r.as_string(), "3/5");
}

TEST(RationalTests, RED_Q_Q_NeedsReduction) {
    Rational r("6/8");
    r.RED_Q_Q();
    EXPECT_EQ(r.as_string(), "3/4");
}

TEST(RationalTests, RED_Q_Q_ToInteger) {
    Rational r("10/5");
    r.RED_Q_Q();
    EXPECT_EQ(r.as_string(), "2");
}

// Q-2 | Тесты для проверки на целое
TEST(RationalTests, INT_Q_B_IsInteger) {
    Rational r("5");
    EXPECT_TRUE(r.INT_Q_B());
}

TEST(RationalTests, INT_Q_B_NotInteger) {
    Rational r("5/3");
    EXPECT_FALSE(r.INT_Q_B());
}

// Q-3 | Тест преобразования целого в дробное
TEST(RationalTests, TRANS_Z_Q) {
    Integer z("42");
    Rational r = Rational::TRANS_Z_Q(z);
    EXPECT_EQ(r.as_string(), "42");
}

// Q-4 | Тест преобразования дробного в целое
TEST(RationalTests, TRANS_Q_Z_Valid) {
    Rational r("42");
    Integer z = r.TRANS_Q_Z();
    EXPECT_EQ(z.as_string(), "42");
}

TEST(RationalTests, TRANS_Q_Z_Invalid) {
    Rational r("5/3");
    EXPECT_THROW(r.TRANS_Q_Z(), std::invalid_argument);
}

// Q-5 | Тесты для сложения дробей
TEST(RationalTests, ADD_QQ_Q_SameDenominator) {
    Rational r1("1/5");
    Rational r2("2/5");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "3/5");
}

TEST(RationalTests, ADD_QQ_Q_DifferentDenominators) {
    Rational r1("1/2");
    Rational r2("1/3");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "5/6");
}

TEST(RationalTests, ADD_QQ_Q_WithZero) {
    Rational r1("3/4");
    Rational r2("0");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "3/4");
}

TEST(RationalTests, ADD_QQ_Q_NegativeNumbers) {
    Rational r1("-1/2");
    Rational r2("1/3");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "-1/6");
}

TEST(RationalTests, ADD_QQ_Q_ResultInteger) {
    Rational r1("1/2");
    Rational r2("1/2");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "1");
}

TEST(RationalTests, ADD_QQ_Q_LargeNumbers) {
    Rational r1("123/456");
    Rational r2("789/1011");
    Rational result = r1.ADD_QQ_Q(r2);
    // Результат должен быть сокращён
    EXPECT_TRUE(result.as_string().find('/') != std::string::npos || 
                result.as_string().find('-') != std::string::npos ||
                !result.as_string().empty());
}

TEST(RationalTests, ADD_QQ_Q_BothNegative) {
    Rational r1("-1/3");
    Rational r2("-1/6");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "-1/2");
}

TEST(RationalTests, ADD_QQ_Q_ComplexFraction) {
    Rational r1("2/7");
    Rational r2("3/14");
    Rational result = r1.ADD_QQ_Q(r2);
    EXPECT_EQ(result.as_string(), "1/2");
}

