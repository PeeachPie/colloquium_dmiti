#include <gtest/gtest.h>
#include "polynomial.hpp"

TEST(PolynomialTests, Create_Default) {
    Polynomial p;
    EXPECT_EQ(p.DEG_P_N(), 0);
}

TEST(PolynomialTests, CreateWithDegree) {
    Polynomial p(5);
    EXPECT_EQ(p.DEG_P_N().as_string(), "5");
}

TEST(PolynomialTests, CreateWithCoeffs) {
    std::vector<std::string> coeffs = {"1", "-2/3", "3/4"};
    std::vector<int> degs = {2, 1, 0};
    Polynomial p(coeffs, degs);
    EXPECT_EQ(p.DEG_P_N().as_string(), "2");
    EXPECT_EQ(p.LED_P_Q().as_string(), "1");
}

TEST(PolynomialTests, CreateWithCoeffs_Exception_SizeMismatch) {
    std::vector<std::string> coeffs = {"1", "-2/3"};
    std::vector<int> degs = {1, 0, 2};
    EXPECT_THROW(Polynomial p(coeffs, degs), std::invalid_argument);
}

TEST(PolynomialTests, CreateWithCoeffs_Exception_Duplicates) {
    std::vector<std::string> coeffs = {"1", "2"};
    std::vector<int> degs = {1, 1};
    EXPECT_THROW(Polynomial p(coeffs, degs), std::invalid_argument);
}

TEST(PolynomialTests, AsString) {
    std::vector<std::string> coeffs = {"1", "-2/3", "3/4"};
    std::vector<int> degs = {2, 1, 0};
    Polynomial p(coeffs, degs);
    EXPECT_EQ(p.as_string(), "1 * x ^ 2 -2/3 * x ^ 1 + 3/4");
}

TEST(PolynomialTests, Norm) {
    std::vector<std::string> coeffs = {"0", "0", "1"};
    std::vector<int> degs = {0, 1, 2};
    Polynomial p(coeffs, degs);
    Polynomial normalized = p.NORM_P_P();
    EXPECT_EQ(normalized.DEG_P_N().as_string(), "2");
}

TEST(PolynomialTests, LED) {
    std::vector<std::string> coeffs = {"1", "-2", "3"};
    std::vector<int> degs = {0, 1, 2};
    Polynomial p(coeffs, degs);
    EXPECT_EQ(p.LED_P_Q().as_string(), "3");
}
