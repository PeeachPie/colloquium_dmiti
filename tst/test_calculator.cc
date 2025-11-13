#include <gtest/gtest.h>
#include "calculator.hpp"

// ==================== ТЕСТЫ СЛОЖЕНИЯ (+) ====================

TEST(CalculatorTest, Addition_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("2x + 3 + 5x + 1");
    EXPECT_EQ(result, "7 * x ^ 1 + 4");
}

TEST(CalculatorTest, Addition_WithNegativeCoefficients) {
    Calculator calc;
    std::string result = calc.simplify_expression("5x^2 - 3x + 2x^2 + 4x - 1");
    EXPECT_EQ(result, "7 * x ^ 2 + 1 * x ^ 1 -1");
}

TEST(CalculatorTest, Addition_WithFractions) {
    Calculator calc;
    std::string result = calc.simplify_expression("1/2x + 1/3x + 1/6x");
    EXPECT_EQ(result, "1 * x ^ 1");
}

TEST(CalculatorTest, Addition_WithBrackets) {
    Calculator calc;
    std::string result = calc.simplify_expression("(2x + 3) + (5x + 1)");
    EXPECT_EQ(result, "7 * x ^ 1 + 4");
}

TEST(CalculatorTest, Addition_ConstantsOnly) {
    Calculator calc;
    std::string result = calc.simplify_expression("5 + 3 + 2");
    EXPECT_EQ(result, "10");
}

// ==================== ТЕСТЫ ВЫЧИТАНИЯ (-) ====================

TEST(CalculatorTest, Subtraction_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("5x^2 + 3x - 2x^2 - x");
    EXPECT_EQ(result, "3 * x ^ 2 + 2 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_WithNegativeResult) {
    Calculator calc;
    std::string result = calc.simplify_expression("2x - 5x");
    EXPECT_EQ(result, "-3 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_WithBrackets) {
    Calculator calc;
    std::string result = calc.simplify_expression("(10x^2 + 5x) - (3x^2 + 2x)");
    EXPECT_EQ(result, "7 * x ^ 2 + 3 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_FromZero) {
    Calculator calc;
    std::string result = calc.simplify_expression("0 - (5x + 3)");
    EXPECT_EQ(result, "-5 * x ^ 1 -3");
}

TEST(CalculatorTest, Subtraction_Complex) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x^2 + 2x + 1) - (x^2 - 2x + 1)");
    EXPECT_EQ(result, "4 * x ^ 1");
}

// ==================== ТЕСТЫ УМНОЖЕНИЯ (*) ====================

TEST(CalculatorTest, Multiplication_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("(2x + 3) * (x + 1)");
    EXPECT_EQ(result, "2 * x ^ 2 + 5 * x ^ 1 + 3");
}

TEST(CalculatorTest, Multiplication_Implicit) {
    Calculator calc;
    std::string result = calc.simplify_expression("(5x + 10)(30x^2 + 10x + 14)");
    EXPECT_EQ(result, "150 * x ^ 3 + 350 * x ^ 2 + 170 * x ^ 1 + 140");
}

TEST(CalculatorTest, Multiplication_WithConstants) {
    Calculator calc;
    std::string result = calc.simplify_expression("5 * (2x + 3)");
    EXPECT_EQ(result, "10 * x ^ 1 + 15");
}

TEST(CalculatorTest, Multiplication_ByZero) {
    Calculator calc;
    std::string result = calc.simplify_expression("(5x + 10) * 0");
    EXPECT_EQ(result, "0");
}

TEST(CalculatorTest, Multiplication_ThreeFactors) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x + 1) * (x + 2) * (x + 3)");
    EXPECT_EQ(result, "1 * x ^ 3 + 6 * x ^ 2 + 11 * x ^ 1 + 6");
}

TEST(CalculatorTest, Multiplication_Complex) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x^2 + x + 1) * (x - 1)");
    EXPECT_EQ(result, "1 * x ^ 3 -1");
}

// ==================== ТЕСТЫ СТЕПЕНИ (^) ====================

TEST(CalculatorTest, Power_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("x^2");
    EXPECT_EQ(result, "1 * x ^ 2");
}

// ==================== ТЕСТЫ НЕЯВНОГО УМНОЖЕНИЯ ====================

TEST(CalculatorTest, ImplicitMultiplication_NumberBeforeX) {
    Calculator calc;
    std::string result = calc.simplify_expression("3x");
    EXPECT_EQ(result, "3 * x ^ 1");
}

TEST(CalculatorTest, ImplicitMultiplication_NumberBeforeBracket) {
    Calculator calc;
    std::string result = calc.simplify_expression("3(x + 1)");
    EXPECT_EQ(result, "3 * x ^ 1 + 3");
}

TEST(CalculatorTest, ImplicitMultiplication_XBeforeBracket) {
    Calculator calc;
    std::string result = calc.simplify_expression("x(x + 1)");
    EXPECT_EQ(result, "1 * x ^ 2 + 1 * x ^ 1");
}

TEST(CalculatorTest, ImplicitMultiplication_BracketBeforeBracket) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x + 1)(x + 2)");
    EXPECT_EQ(result, "1 * x ^ 2 + 3 * x ^ 1 + 2");
}

// ==================== ТЕСТЫ ДЕЛЕНИЯ (/) ====================

TEST(CalculatorTest, Division_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x^2 + 3x + 2) / (x + 1)");
    EXPECT_EQ(result, "1 * x ^ 1 + 2");
}

TEST(CalculatorTest, Division_ExactDivision) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x^2 - 1) / (x - 1)");
    EXPECT_EQ(result, "1 * x ^ 1 + 1");
}

TEST(CalculatorTest, Division_WithRemainder) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x^3 + 2x^2 + x + 1) / (x^2 + 1)");
    EXPECT_EQ(result, "1 * x ^ 1 + 2");
}

TEST(CalculatorTest, Division_ByConstant) {
    Calculator calc;
    std::string result = calc.simplify_expression("(6x^2 + 12x + 6) / 6");
    EXPECT_EQ(result, "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

// ==================== ТЕСТЫ ФУНКЦИЙ ====================

TEST(CalculatorTest, Function_DER_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(x^2 + 3x + 2)");
    EXPECT_EQ(result, "2 * x ^ 1 + 3");
}

TEST(CalculatorTest, Function_DER_HighDegree) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(x^5 + 2x^3 + x)");
    EXPECT_EQ(result, "5 * x ^ 4 + 6 * x ^ 2 + 1");
}

TEST(CalculatorTest, Function_DER_Constant) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(5)");
    EXPECT_EQ(result, "0");
}

TEST(CalculatorTest, Function_GCD_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD(2x^2 + 2, x^2 + 2x + 1)");
    EXPECT_EQ(result, "1");
}

TEST(CalculatorTest, Function_GCD_CommonFactor) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD(x^2 - 1, x^2 - 2x + 1)");
    EXPECT_EQ(result, "1 * x ^ 1 -1");
}

TEST(CalculatorTest, Function_GCD_WithZero) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD(5x + 10, 0)");
    EXPECT_EQ(result, "1 * x ^ 1 + 2");
}
TEST(CalculatorTest, Function_NMR_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("NMR((x - 1)(x - 1))");
    EXPECT_EQ(result, "1 * x ^ 1 -1");
}

TEST(CalculatorTest, Function_FAC_Basic) {
    Calculator calc;
    std::string result = calc.simplify_expression("FAC(6x^2 + 12x + 6)");
    EXPECT_EQ(result, "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

// ==================== ТЕСТЫ КРАЙНИХ СЛУЧАЕВ ПАРСИНГА ====================

TEST(CalculatorTest, Parsing_ImplicitCoefficient) {
    Calculator calc;
    std::string result = calc.simplify_expression("x + 2x + 3x");
    EXPECT_EQ(result, "6 * x ^ 1");
}

TEST(CalculatorTest, Parsing_ImplicitPower) {
    Calculator calc;
    std::string result = calc.simplify_expression("5x + 3x^2 + x");
    EXPECT_EQ(result, "3 * x ^ 2 + 6 * x ^ 1");
}

TEST(CalculatorTest, Parsing_WithoutSpaces) {
    Calculator calc;
    std::string result = calc.simplify_expression("2x^2+3x+1");
    EXPECT_EQ(result, "2 * x ^ 2 + 3 * x ^ 1 + 1");
}

TEST(CalculatorTest, Parsing_WithSpaces) {
    Calculator calc;
    std::string result = calc.simplify_expression("2 * x ^ 2 + 3 * x ^ 1 + 1");
    EXPECT_EQ(result, "2 * x ^ 2 + 3 * x ^ 1 + 1");
}

TEST(CalculatorTest, Parsing_Fractions) {
    Calculator calc;
    std::string result = calc.simplify_expression("1/2x^2 + 1/3x + 1/6");
    EXPECT_EQ(result, "1/2 * x ^ 2 + 1/3 * x ^ 1 + 1/6");
}

TEST(CalculatorTest, Parsing_NegativeFirstTerm) {
    Calculator calc;
    std::string result = calc.simplify_expression("-5x^2 + 3x - 1");
    EXPECT_EQ(result, "-5 * x ^ 2 + 3 * x ^ 1 -1");
}

TEST(CalculatorTest, Parsing_OnlyConstant) {
    Calculator calc;
    std::string result = calc.simplify_expression("42");
    EXPECT_EQ(result, "42");
}

TEST(CalculatorTest, Parsing_OnlyX) {
    Calculator calc;
    std::string result = calc.simplify_expression("x");
    EXPECT_EQ(result, "1 * x ^ 1");
}

TEST(CalculatorTest, Parsing_XSquared) {
    Calculator calc;
    std::string result = calc.simplify_expression("x^2");
    EXPECT_EQ(result, "1 * x ^ 2");
}

// ==================== ТЕСТЫ ПРИОРИТЕТА ОПЕРАЦИЙ ====================

TEST(CalculatorTest, OperatorPrecedence_MultiplicationBeforeAddition) {
    Calculator calc;
    std::string result = calc.simplify_expression("2x + 3 * 5x");
    EXPECT_EQ(result, "17 * x ^ 1");
}

TEST(CalculatorTest, OperatorPrecedence_DivisionBeforeSubtraction) {
    Calculator calc;
    std::string result = calc.simplify_expression("10x^2 - 2x^2 / 2");
    EXPECT_EQ(result, "9 * x ^ 2");
}

TEST(CalculatorTest, OperatorPrecedence_PowerBeforeMultiplication) {
    Calculator calc;
    std::string result = calc.simplify_expression("2x^3 * 3x^2");
    EXPECT_EQ(result, "6 * x ^ 5");
}

TEST(CalculatorTest, OperatorPrecedence_Nested) {
    Calculator calc;
    std::string result = calc.simplify_expression("(2x + 3) * (x + 1) + (x - 1)");
    EXPECT_EQ(result, "2 * x ^ 2 + 6 * x ^ 1 + 2");
}

// ==================== ТЕСТЫ РЕКУРСИВНЫХ ВЫРАЖЕНИЙ ====================

TEST(CalculatorTest, Recursive_ComplexExpression) {
    Calculator calc;
    std::string result = calc.simplify_expression("((x + 1) * (x - 1))(x^2 + 1)");
    EXPECT_EQ(result, "1 * x ^ 4 -1");
}

TEST(CalculatorTest, Recursive_NestedFunctions) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(GCD(x^2 - 1, x^2 - 2x + 1))");
    EXPECT_EQ(result, "1");
}

TEST(CalculatorTest, Recursive_MultipleOperations) {
    Calculator calc;
    std::string result = calc.simplify_expression("((x^2 + x) + (x + 1)) * (x - 1)");
    EXPECT_EQ(result, "1 * x ^ 3 + 1 * x ^ 2 -1 * x ^ 1 -1");
}

// ==================== ТЕСТЫ КОМПЛЕКСНЫХ СЦЕНАРИЕВ ====================

TEST(CalculatorTest, Complex_MixedOperations) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD((x^2 + 2x + 1), (x^2 - 1)) * (x + 1)");
    EXPECT_EQ(result, "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

TEST(CalculatorTest, Complex_ChainOperations) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER((x^2 + x + 1) * (x - 1))");
    EXPECT_EQ(result, "3 * x ^ 2");
}

TEST(CalculatorTest, Complex_AllOperations) {
    Calculator calc;
    std::string result1 = calc.simplify_expression("(x^2 + 1) + (x^2 - 1)");
    std::string result2 = calc.simplify_expression("GCD(x^2 + 1, x^2 - 1)");
    EXPECT_EQ(result1, "2 * x ^ 2");
    EXPECT_EQ(result2, "1");
}

// ==================== ТЕСТЫ ОШИБОК ====================

TEST(CalculatorTest, Error_EmptyExpression) {
    Calculator calc;
    std::string result = calc.simplify_expression("");
    EXPECT_TRUE(result.find("Error") != std::string::npos);
}

TEST(CalculatorTest, Error_MismatchedParentheses) {
    Calculator calc;
    std::string result = calc.simplify_expression("(x + 1");
    EXPECT_TRUE(result.find("Error") != std::string::npos);
}

TEST(CalculatorTest, Error_InvalidToken) {
    Calculator calc;
    std::string result = calc.simplify_expression("x + y");
    EXPECT_TRUE(result.find("Error") != std::string::npos);
}

// ==================== ТЕСТЫ РЕКУРСИВНЫХ ВЫРАЖЕНИЙ С ОТРИЦАТЕЛЬНЫМИ ЧИСЛАМИ ====================

TEST(CalculatorTest, Recursive_ComplexExpression_Negative) {
    Calculator calc;
    std::string result = calc.simplify_expression("((x - 2) * (x + 3))(x^2 - 1)");
    EXPECT_EQ(result, "1 * x ^ 4 + 1 * x ^ 3 -7 * x ^ 2 -1 * x ^ 1 + 6");
}

TEST(CalculatorTest, Recursive_NestedFunctions_Negative) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(GCD(x^2 - 4, x^2 + 2x - 8))");
    EXPECT_EQ(result, "1");
}

TEST(CalculatorTest, Recursive_MultipleOperations_Negative) {
    Calculator calc;
    std::string result = calc.simplify_expression("((x^2 - 2x) + (x - 3)) * (x + 2)");
    EXPECT_EQ(result, "1 * x ^ 3 + 1 * x ^ 2 -5 * x ^ 1 -6");
}

TEST(CalculatorTest, Recursive_NegativeCoefficients) {
    Calculator calc;
    std::string result = calc.simplify_expression("(-2x^2 + 3x - 1)(x - 2)");
    EXPECT_EQ(result, "-2 * x ^ 3 + 7 * x ^ 2 -7 * x ^ 1 + 2");
}

// ==================== ТЕСТЫ КОМПЛЕКСНЫХ СЦЕНАРИЕВ С ОТРИЦАТЕЛЬНЫМИ ЧИСЛАМИ ====================

TEST(CalculatorTest, Complex_MixedOperations_Negative) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD((x^2 - 4x + 4), (x^2 - 4)) * (x - 2)");
    EXPECT_EQ(result, "1 * x ^ 2 -4 * x ^ 1 + 4");
}

TEST(CalculatorTest, Complex_ChainOperations_Negative) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER((x^2 - 3x + 2) * (x + 1))");
    EXPECT_EQ(result, "3 * x ^ 2 -4 * x ^ 1 -1");
}

TEST(CalculatorTest, Complex_AllOperations_Negative) {
    Calculator calc;
    std::string result1 = calc.simplify_expression("(x^2 - 2x) + (x^2 + 3x - 1)");
    std::string result2 = calc.simplify_expression("GCD(x^2 - 1, x^2 + 2x - 3)");
    EXPECT_EQ(result1, "2 * x ^ 2 + 1 * x ^ 1 -1");
    EXPECT_EQ(result2, "1 * x ^ 1 -1");
}

TEST(CalculatorTest, Complex_NegativePolynomials) {
    Calculator calc;
    std::string result = calc.simplify_expression("(-x^3 + 2x^2 - x + 5) + (3x^3 - x^2 + 2x - 1)");
    EXPECT_EQ(result, "2 * x ^ 3 + 1 * x ^ 2 + 1 * x ^ 1 + 4");
}

TEST(CalculatorTest, Complex_DER_WithNegative) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(-2x^3 + 3x^2 - 4x + 1)");
    EXPECT_EQ(result, "-6 * x ^ 2 + 6 * x ^ 1 -4");
}

TEST(CalculatorTest, Complex_GCD_WithNegative) {
    Calculator calc;
    std::string result = calc.simplify_expression("GCD(2x^2 - 3x - 2, x^2 - 4)");
    EXPECT_EQ(result, "1 * x ^ 1 -2");
}

TEST(CalculatorTest, Complex_NestedNegative) {
    Calculator calc;
    std::string result = calc.simplify_expression("DER(GCD(-x^2 + 4, x^2 - 2x))");
    EXPECT_EQ(result, "1");
}

TEST(CalculatorTest, Complex_MultipleNegativeTerms) {
    Calculator calc;
    std::string result = calc.simplify_expression("(-x + 2)(-x - 3)(x - 1)");
    EXPECT_EQ(result, "1 * x ^ 3 -7 * x ^ 1 + 6");
}

TEST(CalculatorTest, Complex_MixedPositiveNegative) {
    Calculator calc;
    std::string result1 = calc.simplify_expression("GCD(x^2 - 5x + 6, x^2 - 9)");
    std::string result2 = calc.simplify_expression("DER(x^3 - 3x^2 + 2x - 1)");
    EXPECT_EQ(result1, "1 * x ^ 1 -3");
    EXPECT_EQ(result2, "3 * x ^ 2 -6 * x ^ 1 + 2");
}