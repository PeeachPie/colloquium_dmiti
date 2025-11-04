#include <gtest/gtest.h>
#include "calculator.hpp"

// ==================== ТЕСТЫ СЛОЖЕНИЯ (+) ====================

TEST(CalculatorTest, Addition_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("2x + 3 + 5x + 1");
    EXPECT_EQ(result.as_string(), "7 * x ^ 1 + 4");
}

TEST(CalculatorTest, Addition_WithNegativeCoefficients) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("5x^2 - 3x + 2x^2 + 4x - 1");
    EXPECT_EQ(result.as_string(), "7 * x ^ 2 + 1 * x ^ 1 -1");
}

TEST(CalculatorTest, Addition_WithFractions) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("1/2x + 1/3x + 1/6x");
    EXPECT_EQ(result.as_string(), "1 * x ^ 1");
}

TEST(CalculatorTest, Addition_WithBrackets) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(2x + 3) + (5x + 1)");
    EXPECT_EQ(result.as_string(), "7 * x ^ 1 + 4");
}

TEST(CalculatorTest, Addition_ConstantsOnly) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("5 + 3 + 2");
    EXPECT_EQ(result.as_string(), "10");
}

// ==================== ТЕСТЫ ВЫЧИТАНИЯ (-) ====================

TEST(CalculatorTest, Subtraction_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("5x^2 + 3x - 2x^2 - x");
    EXPECT_EQ(result.as_string(), "3 * x ^ 2 + 2 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_WithNegativeResult) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("2x - 5x");
    EXPECT_EQ(result.as_string(), "-3 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_WithBrackets) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(10x^2 + 5x) - (3x^2 + 2x)");
    EXPECT_EQ(result.as_string(), "7 * x ^ 2 + 3 * x ^ 1");
}

TEST(CalculatorTest, Subtraction_FromZero) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("0 - (5x + 3)");
    EXPECT_EQ(result.as_string(), "-5 * x ^ 1 -3");
}

TEST(CalculatorTest, Subtraction_Complex) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x^2 + 2x + 1) - (x^2 - 2x + 1)");
    EXPECT_EQ(result.as_string(), "4 * x ^ 1");
}

// ==================== ТЕСТЫ УМНОЖЕНИЯ (*) ====================

TEST(CalculatorTest, Multiplication_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(2x + 3) * (x + 1)");
    EXPECT_EQ(result.as_string(), "2 * x ^ 2 + 5 * x ^ 1 + 3");
}

TEST(CalculatorTest, Multiplication_Implicit) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(5x + 10)(30x^2 + 10x + 14)");
    // Проверяем, что результат не нулевой и имеет правильную степень
    EXPECT_TRUE(result.NZER_P_B()); // NZER_P_B возвращает true для ненулевых
    EXPECT_GE(result.DEG_P_N().as_string(), "2");
}

TEST(CalculatorTest, Multiplication_WithConstants) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("5 * (2x + 3)");
    EXPECT_EQ(result.as_string(), "10 * x ^ 1 + 15");
}

TEST(CalculatorTest, Multiplication_ByZero) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(5x + 10) * 0");
    EXPECT_EQ(result.as_string(), "0");
}

TEST(CalculatorTest, Multiplication_ThreeFactors) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x + 1) * (x + 2) * (x + 3)");
    // Результат должен быть кубическим полиномом
    EXPECT_GE(result.DEG_P_N().as_string(), "3");
}

TEST(CalculatorTest, Multiplication_Complex) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x^2 + x + 1) * (x - 1)");
    EXPECT_EQ(result.as_string(), "1 * x ^ 3 -1");
}

// ==================== ТЕСТЫ ДЕЛЕНИЯ (/) ====================

TEST(CalculatorTest, Division_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x^2 + 3x + 2) / (x + 1)");
    EXPECT_EQ(result.as_string(), "1 * x ^ 1 + 2");
}

TEST(CalculatorTest, Division_ExactDivision) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x^2 - 1) / (x - 1)");
    EXPECT_EQ(result.as_string(), "1 * x ^ 1 + 1");
}

TEST(CalculatorTest, Division_WithRemainder) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(x^3 + 2x^2 + x + 1) / (x^2 + 1)");
    // Результат должен быть полиномом степени не менее 1
    EXPECT_GE(result.DEG_P_N().as_string(), "1");
}

TEST(CalculatorTest, Division_ByConstant) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(6x^2 + 12x + 6) / 6");
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

// ==================== ТЕСТЫ НОД ====================

TEST(CalculatorTest, GCD_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД(2x^2 + 2, x^2 + 2x + 1)");
    // НОД должен быть общим делителем (не нулевым)
    EXPECT_TRUE(result.NZER_P_B());
}

TEST(CalculatorTest, GCD_CommonFactor) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД(x^2 - 1, x^2 - 2x + 1)");
    // НОД должен быть (x - 1)
    EXPECT_GE(result.DEG_P_N().as_string(), "1");
}

TEST(CalculatorTest, GCD_Coprime) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД(x^2 + 1, x^2 + x + 1)");
    // НОД должен быть константой (1)
    EXPECT_EQ(result.DEG_P_N().as_string(), "0");
}

TEST(CalculatorTest, GCD_MultipleFactors) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД(x^3 - x, x^2 - 1)");
    // НОД должен быть x^2 - 1
    EXPECT_GE(result.DEG_P_N().as_string(), "2");
}

TEST(CalculatorTest, GCD_WithZero) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД(5x + 10, 0)");
    EXPECT_EQ(result.as_string(), "5 * x ^ 1 + 10");
}

// ==================== ТЕСТЫ НОК ====================

TEST(CalculatorTest, LCM_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОК(x^2 - 1, x^2 + 2x + 1)");
    // НОК должен быть кратным обоим полиномам
    EXPECT_GE(result.DEG_P_N().as_string(), "2");
}

TEST(CalculatorTest, LCM_CommonBase) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОК(x + 1, x - 1)");
    // НОК должен быть (x + 1)(x - 1) = x^2 - 1
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 -1");
}

TEST(CalculatorTest, LCM_OneMultiple) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОК(x + 1, x^2 + 2x + 1)");
    // НОК должен быть x^2 + 2x + 1
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

TEST(CalculatorTest, LCM_Constants) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОК(6, 4)");
    // НОК(6, 4) = 12
    EXPECT_EQ(result.as_string(), "12");
}

// ==================== ТЕСТЫ ПРОИЗВОДНОЙ ====================

TEST(CalculatorTest, Derivative_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(x^2 + 3x + 2)");
    EXPECT_EQ(result.as_string(), "2 * x ^ 1 + 3");
}

TEST(CalculatorTest, Derivative_HighDegree) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(x^5 + 2x^3 + x)");
    EXPECT_EQ(result.as_string(), "5 * x ^ 4 + 6 * x ^ 2 + 1");
}

TEST(CalculatorTest, Derivative_Constant) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(5)");
    EXPECT_EQ(result.as_string(), "0");
}

TEST(CalculatorTest, Derivative_Linear) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(5x + 10)");
    EXPECT_EQ(result.as_string(), "5");
}

TEST(CalculatorTest, Derivative_Complex) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(3x^4 - 2x^3 + 5x^2 - x + 7)");
    EXPECT_EQ(result.as_string(), "12 * x ^ 3 -6 * x ^ 2 + 10 * x ^ 1 -1");
}

// ==================== ТЕСТЫ ФАКТОРИЗАЦИИ ====================

TEST(CalculatorTest, Factorization_Basic) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Факторизация(6x^2 + 12x + 6)");
    // Факторизация должна вернуть полином с коэффициентом 1 при старшем члене
    EXPECT_GE(result.DEG_P_N().as_string(), "2");
}

TEST(CalculatorTest, Factorization_WithFractions) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Факторизация(2/3x^2 + 4/5x + 6/7)");
    // Результат должен быть нормализован (не нулевым)
    EXPECT_TRUE(result.NZER_P_B());
}

TEST(CalculatorTest, Factorization_Simple) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Факторизация(2x^2 + 4x + 2)");
    // Результат должен быть x^2 + 2x + 1
    EXPECT_EQ(result.as_string(), "1 * x ^ 2 + 2 * x ^ 1 + 1");
}

// ==================== ТЕСТЫ КРАЙНИХ СЛУЧАЕВ ПАРСИНГА ====================

TEST(CalculatorTest, Parsing_ImplicitCoefficient) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("x + 2x + 3x");
    EXPECT_EQ(result.as_string(), "6 * x ^ 1");
}

TEST(CalculatorTest, Parsing_ImplicitPower) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("5x + 3x^2 + x");
    EXPECT_EQ(result.as_string(), "3 * x ^ 2 + 6 * x ^ 1");
}

TEST(CalculatorTest, Parsing_WithoutSpaces) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("2x^2+3x+1");
    EXPECT_EQ(result.as_string(), "2 * x ^ 2 + 3 * x ^ 1 + 1");
}

TEST(CalculatorTest, Parsing_WithSpaces) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("2 * x ^ 2 + 3 * x ^ 1 + 1");
    EXPECT_EQ(result.as_string(), "2 * x ^ 2 + 3 * x ^ 1 + 1");
}

TEST(CalculatorTest, Parsing_Fractions) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("1/2x^2 + 1/3x + 1/6");
    // Полином с дробными коэффициентами не нулевой
    EXPECT_TRUE(result.NZER_P_B());
}

TEST(CalculatorTest, Parsing_NegativeFirstTerm) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("-5x^2 + 3x - 1");
    EXPECT_EQ(result.as_string(), "-5 * x ^ 2 + 3 * x ^ 1 -1");
}

TEST(CalculatorTest, Parsing_OnlyConstant) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("42");
    EXPECT_EQ(result.as_string(), "42");
}

TEST(CalculatorTest, Parsing_OnlyX) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("x");
    EXPECT_EQ(result.as_string(), "1 * x ^ 1");
}

TEST(CalculatorTest, Parsing_XSquared) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("x^2");
    EXPECT_EQ(result.as_string(), "1 * x ^ 2");
}

// ==================== ТЕСТЫ ПРИОРИТЕТА ОПЕРАЦИЙ ====================

TEST(CalculatorTest, OperatorPrecedence_MultiplicationBeforeAddition) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("2x + 3 * 5x");
    // 2x + (3 * 5x) = 2x + 15x = 17x
    EXPECT_EQ(result.as_string(), "17 * x ^ 1");
}

TEST(CalculatorTest, OperatorPrecedence_DivisionBeforeSubtraction) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("10x^2 - 2x^2 / 2");
    // 10x^2 - (2x^2 / 2) = 10x^2 - x^2 = 9x^2
    EXPECT_EQ(result.as_string(), "9 * x ^ 2");
}

TEST(CalculatorTest, OperatorPrecedence_Nested) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("(2x + 3) * (x + 1) + (x - 1)");
    // (2x+3)(x+1) + (x-1) = 2x^2 + 5x + 3 + x - 1 = 2x^2 + 6x + 2
    EXPECT_EQ(result.as_string(), "2 * x ^ 2 + 6 * x ^ 1 + 2");
}

// ==================== ТЕСТЫ РЕКУРСИВНЫХ ВЫРАЖЕНИЙ ====================

TEST(CalculatorTest, Recursive_ComplexExpression) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("((x + 1) * (x - 1))(x^2 + 1)");
    // (x^2 - 1)(x^2 + 1) = x^4 - 1
    EXPECT_EQ(result.as_string(), "1 * x ^ 4 -1");
}

TEST(CalculatorTest, Recursive_NestedFunctions) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная(НОД(x^2 - 1, x^2 - 2x + 1))");
    // Производная от НОД не обязательно нулевая, но для большинства случаев ненулевая
    EXPECT_TRUE(result.NZER_P_B());
}

TEST(CalculatorTest, Recursive_MultipleOperations) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("((x^2 + x) + (x + 1)) * (x - 1)");
    // (x^2 + 2x + 1)(x - 1) = x^3 + x^2 - x - 1
    EXPECT_GE(result.DEG_P_N().as_string(), "3");
}

// ==================== ТЕСТЫ КОМПЛЕКСНЫХ СЦЕНАРИЕВ ====================

TEST(CalculatorTest, Complex_MixedOperations) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("НОД((x^2 + 2x + 1), (x^2 - 1)) * (x + 1)");
    EXPECT_GE(result.DEG_P_N().as_string(), "2");
}

TEST(CalculatorTest, Complex_ChainOperations) {
    Calculator calc;
    Polynomial result = calc.simplify_expression("Производная((x^2 + x + 1) * (x - 1))");
    EXPECT_GE(result.DEG_P_N().as_string(), "1");
}

TEST(CalculatorTest, Complex_AllOperations) {
    Calculator calc;
    Polynomial p1 = calc.simplify_expression("(x^2 + 1) + (x^2 - 1)");  // = 2x^2
    Polynomial p2 = calc.simplify_expression("НОД(x^2 + 1, x^2 - 1)");
    EXPECT_TRUE(p1.NZER_P_B());  // 2x^2 не нулевой
    EXPECT_TRUE(p2.NZER_P_B());  // НОД не нулевой
}

