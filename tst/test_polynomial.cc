#include <gtest/gtest.h>
#include "polynomial.hpp"

TEST(PolynomialTest, DEG_P_N) {
    std::vector<std::pair<int, std::string>> terms = {{5, "1"}, {3, "2"}, {1, "3"}};
    Polynomial p(terms);
    EXPECT_EQ(p.DEG_P_N().as_string(), "5");
    
    Polynomial zero;
    EXPECT_EQ(zero.DEG_P_N().as_string(), "0");
}

TEST(PolynomialTest, MUL_PP_P) {
    std::vector<std::pair<int, std::string>> terms1 = {{1, "2"}, {0, "1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{1, "3"}, {0, "2"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.MUL_PP_P(p2);
    EXPECT_EQ(result.as_string(), "6 * x ^ 2 + 7 * x ^ 1 + 2");
}

TEST(PolynomialTest, DIV_PP_P) {
    std::vector<std::pair<int, std::string>> terms1 = {{2, "1"}, {1, "-3"}, {0, "2"}};
    std::vector<std::pair<int, std::string>> terms2 = {{1, "1"}, {0, "-1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.DIV_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1 * x ^ 1 -2");
}

TEST(PolynomialTest, MOD_PP_P) {
    std::vector<std::pair<int, std::string>> terms1 = {{3, "1"}, {1, "-2"}, {0, "1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{2, "1"}, {0, "1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.MOD_PP_P(p2);
    EXPECT_EQ(result.as_string(), "-3 * x ^ 1 + 1");
}

TEST(PolynomialTest, GCF_PP_P_SimpleCase) {
    std::vector<std::pair<int, std::string>> terms1 = {{2, "1"}, {1, "-3"}, {0, "2"}};
    std::vector<std::pair<int, std::string>> terms2 = {{2, "1"}, {1, "-4"}, {0, "3"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1 * x ^ 1 -1");
}

TEST(PolynomialTest, GCF_PP_P_CommonQuadratic) {
    std::vector<std::pair<int, std::string>> terms1 = {{3, "1"}, {2, "-2"}, {1, "-1"}, {0, "2"}};
    std::vector<std::pair<int, std::string>> terms2 = {{3, "1"}, {2, "-1"}, {1, "-4"}, {0, "4"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_TRUE(result.as_string() == "-1 * x ^ 2 + 3 * x ^ 1 -2" || result.as_string() == "1 * x ^ 2 -3 * x ^ 1 + 2");
}

TEST(PolynomialTest, GCF_PP_P_OneIsMultiple) {
    std::vector<std::pair<int, std::string>> terms1 = {{2, "1"}, {0, "-1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{4, "1"}, {0, "-1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 -1");
}

TEST(PolynomialTest, GCF_PP_P_NoCommonFactors) {
    std::vector<std::pair<int, std::string>> terms1 = {{2, "1"}, {0, "1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{2, "1"}, {1, "1"}, {0, "1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1");
}

TEST(PolynomialTest, GCF_PP_P_WithZeroPolynomial) {
    std::vector<std::pair<int, std::string>> terms1 = {{2, "1"}, {1, "1"}, {0, "1"}};
    Polynomial p1(terms1);
    Polynomial p2;
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 + 1 * x ^ 1 + 1");
}

TEST(PolynomialTest, GCF_PP_P_BothZero) {
    Polynomial p1;
    Polynomial p2;
    
    EXPECT_THROW(p1.GCF_PP_P(p2), std::invalid_argument);
}

TEST(PolynomialTest, GCF_PP_P_ConstantPolynomials) {
    std::vector<std::pair<int, std::string>> terms1 = {{0, "4"}};
    std::vector<std::pair<int, std::string>> terms2 = {{0, "6"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1"); // Договоримся так
}

TEST(PolynomialTest, GCF_PP_P_YourOriginalTest) {
    std::vector<std::pair<int, std::string>> terms1 = {{3, "1"}, {2, "-1"}, {1, "-1"}, {0, "1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{2, "1"}, {1, "-2"}, {0, "1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial result = p1.GCF_PP_P(p2);
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 -2 * x ^ 1 + 1");
}

TEST(PolynomialTest, NMR_P_P_MultipleRoots) {
    // (x-1)^3 * (x-2)^2
    std::vector<std::pair<int, std::string>> terms = {
        {5, "1"}, 
        {4, "-7"}, 
        {3, "19"}, 
        {2, "-25"}, 
        {1, "16"}, 
        {0, "-4"}
    };
    Polynomial p(terms);
    
    Polynomial result = p.NMR_P_P();
    
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 -3 * x ^ 1 + 2");
}

TEST(PolynomialTest, NORM_P_P) {
    std::vector<std::pair<int, std::string>> terms = {{2, "2"}, {1, "0"}, {0, "3"}};
    Polynomial p(terms);
    
    Polynomial result = p.NORM_P_P();
    EXPECT_EQ(result.as_string(), "2 * x ^ 2 + 3");
}

TEST(PolynomialTest, EdgeCases) {
    std::vector<std::pair<int, std::string>> terms1 = {{1, "1"}};
    std::vector<std::pair<int, std::string>> terms2 = {{2, "1"}};
    Polynomial p1(terms1);
    Polynomial p2(terms2);
    
    Polynomial div_result = p1.DIV_PP_P(p2);
    EXPECT_EQ(div_result.as_string(), "0");
    
    Polynomial mod_result = p1.MOD_PP_P(p2);
    EXPECT_EQ(mod_result.as_string(), "1 * x ^ 1");
}

TEST(PolynomialTest, ZeroPolynomialOperations) {
    Polynomial zero;
    std::vector<std::pair<int, std::string>> terms = {{2, "1"}, {0, "1"}};
    Polynomial p(terms);
    
    Polynomial mul_result = p.MUL_PP_P(zero);
    EXPECT_FALSE(mul_result.NZER_P_B());
    
    Polynomial gcd_result = p.GCF_PP_P(zero);
    EXPECT_EQ(gcd_result.as_string(), p.as_string());
}

TEST(PolynomialTest, FAC_P_Q) {
    std::vector<std::pair<int, std::string>> terms = {
        {3, "2/3"}, 
        {2, "4/5"}, 
        {1, "6/7"}, 
        {0, "8/9"}
    };
    Polynomial p(terms);
    
    auto [rational_part, polynomial_part] = p.FAC_P_Q();
    
    Polynomial reconstructed = polynomial_part.MUL_PQ_P(rational_part);
    EXPECT_EQ(reconstructed.as_string(), p.as_string());
    
    std::vector<std::pair<int, std::string>> terms2 = {
        {2, "2"}, 
        {1, "4"}, 
        {0, "6"}
    };
    Polynomial p2(terms2);
    
    auto [rational_part2, polynomial_part2] = p2.FAC_P_Q();
    EXPECT_EQ(rational_part2.as_string(), "2");
    EXPECT_EQ(polynomial_part2.as_string(), "1 * x ^ 2 + 2 * x ^ 1 + 3");
    
    std::vector<std::pair<int, std::string>> terms3 = {
        {2, "-1/2"}, 
        {1, "3/4"}, 
        {0, "-5/6"}
    };
    Polynomial p3(terms3);
    
    auto [rational_part3, polynomial_part3] = p3.FAC_P_Q();
    
    Polynomial reconstructed3 = polynomial_part3.MUL_PQ_P(rational_part3);
    EXPECT_EQ(reconstructed3.as_string(), p3.as_string());
    
    bool all_coefficients_integer = true;

    Polynomial zero;
    EXPECT_THROW(zero.FAC_P_Q(), std::invalid_argument);
}

TEST(PolynomialTest, FAC_P_Q_Detailed) {
    std::vector<std::pair<int, std::string>> terms = {
        {2, "2/3"}, 
        {1, "4/5"}, 
        {0, "6/7"}
    };
    Polynomial p(terms);
    
    auto [rational_part, polynomial_part] = p.FAC_P_Q();
    
    EXPECT_TRUE(rational_part == Rational("2/105") || rational_part == Rational("-2/105"));
    
    EXPECT_EQ(polynomial_part.as_string(), "35 * x ^ 2 + 42 * x ^ 1 + 45");
    
    Polynomial reconstructed = polynomial_part.MUL_PQ_P(rational_part);
    EXPECT_EQ(reconstructed.as_string(), p.as_string());
}