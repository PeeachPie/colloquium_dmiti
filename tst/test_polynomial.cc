#include <gtest/gtest.h>
#include "polynomial.hpp"

TEST(PolynomialTest, Create_Default) {
    Polynomial p;
    EXPECT_FALSE(p.NZER_P_B());
    EXPECT_EQ(p.DEG_P_N().as_string(), "0");
}

TEST(PolynomialTest, Create) {
    std::vector<std::pair<int, std::string>> terms = {{10, "1"}, {8, "-2/3"}, {6, "4/5"}, {4, "-6/7"}, {2, "7/8"}};
    Polynomial p(terms);
    EXPECT_EQ(p.DEG_P_N().as_string(), "10");
    EXPECT_EQ(p.LED_P_Q().as_string(), "1");
}

TEST(PolynomialTest, as_string) {
    std::vector<std::pair<int, std::string>> terms = {{10, "1"}, {8, "-2/3"}, {6, "4/5"}, {4, "-6/7"}, {2, "7/8"}};
    Polynomial p(terms);
    EXPECT_EQ(p.as_string(), "1 * x ^ 10 -2/3 * x ^ 8 + 4/5 * x ^ 6 -6/7 * x ^ 4 + 7/8 * x ^ 2");
}

TEST(PolynomialTest, NZER_P_B) {
    Polynomial p;
    std::vector<std::pair<int, std::string>> terms = {{2, "1"}, {1, "2"}};
    Polynomial q(terms);
    EXPECT_FALSE(p.NZER_P_B());
    EXPECT_TRUE(q.NZER_P_B());
}

TEST(PolynomialTest, MUL_PQ_P) {
    std::vector<std::pair<int, std::string>> terms = {{4, "-1/9"}, {2, "2/5"}, {1, "6/13"}, {0, "-9/44"}};
    Polynomial p(terms);
    Rational r("-112/97");
    Polynomial result = p.MUL_PQ_P(r);
    EXPECT_EQ(result.as_string(), "112/873 * x ^ 4 -224/485 * x ^ 2 -672/1261 * x ^ 1 + 252/1067");
}

TEST(PolynomialTest, MUL_Pxk_P) {
    std::vector<std::pair<int, std::string>> terms = {{4, "-1/9"}, {2, "2/5"}, {1, "6/13"}, {0, "-9/44"}};
    Polynomial p(terms);
    int d = 4;
    Polynomial result = p.MUL_Pxk_P(d);
    EXPECT_EQ(result.as_string(), "-1/9 * x ^ 8 + 2/5 * x ^ 6 + 6/13 * x ^ 5 -9/44 * x ^ 4");
}

TEST(PolynomialTest, FAC_P_Q) {
    std::vector<std::pair<int, std::string>> terms = {{7, "-10/9"}, {5, "-15"}, {3, "30/13"}, {0, "-25/26"}};
    Polynomial p(terms);
    std::pair<Rational, Polynomial> result = p.FAC_P_Q();
    EXPECT_FALSE(result.first.SUB_QQ_Q(Rational("5/234")).NZER_Q_B());
    EXPECT_EQ(result.second.as_string(), "-52 * x ^ 7 -702 * x ^ 5 + 108 * x ^ 3 -45");
}

TEST(PolynomialTest, DER_P_P) {
    std::vector<std::pair<int, std::string>> terms = {{7, "-10/7"}, {5, "-7/10"}, {3, "29/15"}, {0, "-25/26"}};
    Polynomial p(terms);
    Polynomial result = p.DER_P_P();
    EXPECT_EQ(result.DEG_P_N().as_string(), "6");
    EXPECT_EQ(result.as_string(), "-10 * x ^ 6 -7/2 * x ^ 4 + 29/5 * x ^ 2");
}

TEST(PolynomialTests, LED_P_Q) {
    std::vector<std::pair<int, std::string>> terms = {{0, "1"}, {1, "-2"}, {2, "3"}};
    Polynomial p(terms);
    EXPECT_EQ(p.LED_P_Q().as_string(), "3");
}

TEST(PolynomialTests, Add_PP_P) {
    std::vector<std::pair<int, std::string>> terms1 = {{0, "1"}, {1, "2"}, {2, "3"}};
    Polynomial p1(terms1);

    std::vector<std::pair<int, std::string>> terms2 = {{0, "2"}, {1, "-1"}, {2, "4"}};
    Polynomial p2(terms2);

    Polynomial result = p1.ADD_PP_P(p2);
    EXPECT_EQ(result.as_string(), "7 * x ^ 2 + 1 * x ^ 1 + 3");
}

TEST(PolynomialTests, Sub_PP_P) {
    std::vector<std::pair<int, std::string>> terms1 = {{0, "1"}, {1, "2"}, {2, "3"}};
    Polynomial p1(terms1);

    std::vector<std::pair<int, std::string>> terms2 = {{0, "2"}, {1, "-1"}, {2, "4"}};
    Polynomial p2(terms2);

    Polynomial result = p1.SUB_PP_P(p2);
    EXPECT_EQ(result.as_string(), "-1 * x ^ 2 + 3 * x ^ 1 -1");
}