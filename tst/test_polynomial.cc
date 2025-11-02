#include <gtest/gtest.h>
#include "polynomial.hpp"

TEST(PolynomialTest, Create_Default) {
    Polynomial p;
    EXPECT_FALSE(p.NZER_P_B());
    // EXPECT_EQ(p.DEG_P_N(), 0); - метод DEG_P_N() еще не реализован
}

TEST(PolynomialTest, Create_Default_Long) {
    Polynomial p(79);
    EXPECT_FALSE(p.NZER_P_B());
    // EXPECT_EQ(p.DEG_P_N(), 79); - метод DEG_P_N() еще не реализован
}

TEST(PolynomialTest, Create) {
    std::vector<std::string> coeffs = {"1", "-2/3", "4/5", "-6/7", "7/8"};
    std::vector<int> degs = {10, 8, 6, 4, 2};
    Polynomial p(coeffs, degs);
    // EXPECT_EQ(p.DEG_P_N(), 10); - метод DEG_P_N() еще не реализован
    // EXPECT_EQ(p.LED_P_Q(), Rational("1")) - метод LED_P_Q() еще не реализован
}

TEST(PolynomialTest, Create_Exception_TooFewDegs) {
    std::vector<std::string> coeffs = {"1", "-2/3", "4/5", "-6/7", "7/8"};
    std::vector<int> degs = {10, 2};
    EXPECT_THROW(Polynomial p(coeffs, degs), std::invalid_argument);
}

TEST(PolynomialTest, Create_Exception_TooFewCoeffs) {
    std::vector<std::string> coeffs = {"1", "-2/3"};
    std::vector<int> degs = {10, 9, 8, 7, 6, 5};
    EXPECT_THROW(Polynomial p(coeffs, degs), std::invalid_argument);
}

TEST(PolynomialTest, Create_Exception_Duplicates) {
    std::vector<std::string> coeffs = {"1", "-2/3", "4/5", "-6/7", "7/8"};
    std::vector<int> degs = {10, 10, 2, 5, 10};
    EXPECT_THROW(Polynomial p(coeffs, degs), std::invalid_argument);
}

TEST(PolynomialTest, as_string) {
    std::vector<std::string> coeffs = {"1", "-2/3", "4/5", "-6/7", "7/8"};
    std::vector<int> degs = {10, 8, 6, 4, 2};
    Polynomial p(coeffs, degs);
    EXPECT_EQ(p.as_string(), "1 * x ^ 10 -2/3 * x ^ 8 + 4/5 * x ^ 6 -6/7 * x ^ 4 + 7/8 * x ^ 2");
}

TEST(PolynomialTest, NZER_P_B) {
    Polynomial p;
    std::vector<std::string> coeffs = {"1", "2"};
    std::vector<int> degs = {2, 1};
    Polynomial q(coeffs, degs);
    EXPECT_FALSE(p.NZER_P_B());
    EXPECT_TRUE(q.NZER_P_B());
}

TEST(PolynomialTest, MUL_PQ_P) {
    std::vector<std::string> coeffs = {"-1/9", "2/5", "6/13", "-9/44"};
    std::vector<int> degs = {4, 2, 1, 0};
    Polynomial p(coeffs, degs);
    Rational r("-112/97");
    Polynomial result = p.MUL_PQ_P(r);
    EXPECT_EQ(result.as_string(), "112/873 * x ^ 4 -224/485 * x ^ 2 -672/1261 * x ^ 1 + 252/1067");
}

TEST(PolynomialTest, MUL_Pxk_P) {
    std::vector<std::string> coeffs = {"-1/9", "2/5", "6/13", "-9/44"};
    std::vector<int> degs = {4, 2, 1, 0};
    Polynomial p(coeffs, degs);
    int d = 4;
    Polynomial result = p.MUL_Pxk_P(d);
    EXPECT_EQ(result.as_string(), "-1/9 * x ^ 8 + 2/5 * x ^ 6 + 6/13 * x ^ 5 -9/44 * x ^ 4");
}

TEST(PolynomialTest, FAC_P_Q) {
    std::vector<std::string> coeffs = {"-10/9", "-15", "30/13", "-25/26"};
    std::vector<int> degs = {7, 5, 3, 0};
    Polynomial p(coeffs, degs);
    std::pair<Rational, Polynomial> result = p.FAC_P_Q();
    EXPECT_FALSE(result.first.SUB_QQ_Q(Rational("5/234")).NZER_Q_B());
    EXPECT_EQ(result.second.as_string(), "-52 * x ^ 7 -702 * x ^ 5 + 108 * x ^ 3 -45");
}

TEST(PolynomialTest, DER_P_P) {
    std::vector<std::string> coeffs = {"-10/7", "-7/10", "29/15", "-25/26"};
    std::vector<int> degs = {7, 5, 3, 0};
    Polynomial p(coeffs, degs);
    Polynomial result = p.DER_P_P();
    // EXPECT_EQ(result.DEG_P_N(), 6); - метод DEG_P_N() еще не реализован
    EXPECT_EQ(result.as_string(), "-10 * x ^ 6 -7/2 * x ^ 4 + 29/5 * x ^ 2");
}