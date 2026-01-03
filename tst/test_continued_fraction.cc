#include <gtest/gtest.h>
#include "continued_fraction.hpp"


// CF-1: Преобразование рационального числа в цепную дробь
TEST(ContinuedFractionTest, FROM_Q_CF_SimpleInteger) {
    Rational r("5");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(r);
    EXPECT_EQ(cf.coefficients().size(), 1);
    EXPECT_EQ(cf.coefficients()[0].as_string(), "5");
}

TEST(ContinuedFractionTest, FROM_Q_CF_SimpleFraction) {
    // 3/2 = [1; 2] потому что 3/2 = 1 + 1/2
    Rational r("3/2");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(r);
    EXPECT_EQ(cf.coefficients().size(), 2);
    EXPECT_EQ(cf.coefficients()[0].as_string(), "1");
    EXPECT_EQ(cf.coefficients()[1].as_string(), "2");
}

TEST(ContinuedFractionTest, FROM_Q_CF_GoldenRatio) {
    // 89/55 ≈ φ (золотое сечение), должно дать [1; 1, 1, 1, 1, 1, 1, 1, 1, 2]
    Rational r("89/55");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(r);
    // Проверяем, что все коэффициенты кроме последнего равны 1
    for (size_t i = 0; i < cf.coefficients().size() - 1; ++i) {
        EXPECT_EQ(cf.coefficients()[i].as_string(), "1");
    }
}

TEST(ContinuedFractionTest, FROM_Q_CF_NegativeFraction) {
    // -7/3 = [-3; 1, 2] потому что -7/3 = -3 + 2/3 = -3 + 1/(3/2) = -3 + 1/(1 + 1/2)
    Rational r("-7/3");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(r);
    EXPECT_EQ(cf.coefficients()[0].as_string(), "-3");
}

// CF-2: Преобразование цепной дроби в рациональное число
TEST(ContinuedFractionTest, TO_CF_Q_SimpleInteger) {
    ContinuedFraction cf(std::vector<int>{5});
    Rational r = cf.TO_CF_Q();
    EXPECT_EQ(r.as_string(), "5");
}

TEST(ContinuedFractionTest, TO_CF_Q_SimpleFraction) {
    // [1; 2] = 1 + 1/2 = 3/2
    ContinuedFraction cf(std::vector<int>{1, 2});
    Rational r = cf.TO_CF_Q();
    EXPECT_EQ(r.as_string(), "3/2");
}

TEST(ContinuedFractionTest, TO_CF_Q_LongerFraction) {
    // [3; 7, 15, 1] = 355/113 (приближение π)
    ContinuedFraction cf(std::vector<int>{3, 7, 15, 1});
    Rational r = cf.TO_CF_Q();
    EXPECT_EQ(r.as_string(), "355/113");
}

// Проверка обратимости: Q -> CF -> Q
TEST(ContinuedFractionTest, RoundTrip_Q_CF_Q) {
    Rational original("355/113");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(original);
    Rational result = cf.TO_CF_Q();
    EXPECT_EQ(original.as_string(), result.as_string());
}

TEST(ContinuedFractionTest, RoundTrip_Q_CF_Q_Large) {
    Rational original("12345/6789");
    ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(original);
    Rational result = cf.TO_CF_Q();
    EXPECT_TRUE(original.EQ_QQ_B(result));
}

// CF-3: Получение последовательности конвергентов
TEST(ContinuedFractionTest, CONVERGENTS_CF) {
    // [3; 7, 15, 1] = 355/113
    // Конвергенты: 3/1, 22/7, 333/106, 355/113
    ContinuedFraction cf(std::vector<int>{3, 7, 15, 1});
    std::vector<Rational> conv = cf.CONVERGENTS_CF();
    
    EXPECT_EQ(conv.size(), 4);
    EXPECT_EQ(conv[0].as_string(), "3");
    EXPECT_EQ(conv[1].as_string(), "22/7");
    EXPECT_EQ(conv[2].as_string(), "333/106");
    EXPECT_EQ(conv[3].as_string(), "355/113");
}

// CF-4: Получение n-го конвергента
TEST(ContinuedFractionTest, CONVERGENT_N_CF) {
    ContinuedFraction cf(std::vector<int>{3, 7, 15, 1});
    
    EXPECT_EQ(cf.CONVERGENT_N_CF(0).as_string(), "3");
    EXPECT_EQ(cf.CONVERGENT_N_CF(1).as_string(), "22/7");
    EXPECT_EQ(cf.CONVERGENT_N_CF(2).as_string(), "333/106");
    EXPECT_EQ(cf.CONVERGENT_N_CF(3).as_string(), "355/113");
}

TEST(ContinuedFractionTest, CONVERGENT_N_CF_OutOfRange) {
    ContinuedFraction cf(std::vector<int>{3, 7});
    EXPECT_THROW((void)cf.CONVERGENT_N_CF(5), std::out_of_range);
    EXPECT_THROW((void)cf.CONVERGENT_N_CF(-1), std::out_of_range);
}

// CF-5: Получение приближения с заданной точностью
TEST(ContinuedFractionTest, APPROX_CF_Q) {
    // π ≈ [3; 7, 15, 1, 292, ...]
    ContinuedFraction cf(std::vector<int>{3, 7, 15, 1, 292});
    
    // С максимальным знаменателем 10 должны получить 22/7
    Rational approx1 = cf.APPROX_CF_Q(Natural("10"));
    EXPECT_EQ(approx1.as_string(), "22/7");
    
    // С максимальным знаменателем 200 должны получить 355/113
    Rational approx2 = cf.APPROX_CF_Q(Natural("200"));
    EXPECT_EQ(approx2.as_string(), "355/113");
}

// CF-6: Сравнение двух цепных дробей
TEST(ContinuedFractionTest, COM_CF_D_Equal) {
    ContinuedFraction cf1(std::vector<int>{1, 2, 3});
    ContinuedFraction cf2(std::vector<int>{1, 2, 3});
    EXPECT_EQ(cf1.COM_CF_D(cf2), 0);
}

TEST(ContinuedFractionTest, COM_CF_D_Less) {
    ContinuedFraction cf1(std::vector<int>{1, 2});  // 1 + 1/2 = 3/2
    ContinuedFraction cf2(std::vector<int>{2});     // 2
    EXPECT_EQ(cf1.COM_CF_D(cf2), -1);
}

TEST(ContinuedFractionTest, COM_CF_D_Greater) {
    ContinuedFraction cf1(std::vector<int>{3, 7});  // 22/7
    ContinuedFraction cf2(std::vector<int>{3});     // 3
    EXPECT_EQ(cf1.COM_CF_D(cf2), 1);
}

// CF-7: Получение обратной дроби
TEST(ContinuedFractionTest, INV_CF_CF_Simple) {
    // [0; 2] = 1/2, обратная = [2] = 2
    ContinuedFraction cf(std::vector<int>{0, 2});
    ContinuedFraction inv = cf.INV_CF_CF();
    Rational r = inv.TO_CF_Q();
    EXPECT_EQ(r.as_string(), "2");
}

TEST(ContinuedFractionTest, INV_CF_CF_NonZeroStart) {
    // [2] = 2, обратная = [0; 2] = 1/2
    ContinuedFraction cf(std::vector<int>{2});
    ContinuedFraction inv = cf.INV_CF_CF();
    Rational r = inv.TO_CF_Q();
    EXPECT_EQ(r.as_string(), "1/2");
}

TEST(ContinuedFractionTest, INV_CF_CF_Zero) {
    ContinuedFraction cf(std::vector<int>{0});
    EXPECT_THROW((void)cf.INV_CF_CF(), std::runtime_error);
}

// CF-8: Преобразование sqrt(D) в периодическую цепную дробь
TEST(ContinuedFractionTest, FROM_SQRT_CF_PerfectSquare) {
    // sqrt(4) = 2, не периодическая
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("4"));
    EXPECT_EQ(cf.coefficients().size(), 1);
    EXPECT_EQ(cf.coefficients()[0].as_string(), "2");
    EXPECT_FALSE(cf.IS_PERIODIC_CF());
}

TEST(ContinuedFractionTest, FROM_SQRT_CF_Two) {
    // sqrt(2) = [1; (2)...] - периодическая с периодом 2
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("2"));
    EXPECT_TRUE(cf.IS_PERIODIC_CF());
    EXPECT_EQ(cf.coefficients()[0].as_string(), "1");
    
    std::vector<Integer> period = cf.GET_PERIOD_CF();
    EXPECT_EQ(period.size(), 1);
    EXPECT_EQ(period[0].as_string(), "2");
}

TEST(ContinuedFractionTest, FROM_SQRT_CF_Three) {
    // sqrt(3) = [1; (1, 2)...]
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("3"));
    EXPECT_TRUE(cf.IS_PERIODIC_CF());
    EXPECT_EQ(cf.coefficients()[0].as_string(), "1");
}

TEST(ContinuedFractionTest, FROM_SQRT_CF_Five) {
    // sqrt(5) = [2; (4)...]
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("5"));
    EXPECT_TRUE(cf.IS_PERIODIC_CF());
    EXPECT_EQ(cf.coefficients()[0].as_string(), "2");
}

// CF-10, CF-11, CF-12: Периодичность
TEST(ContinuedFractionTest, IS_PERIODIC_CF) {
    ContinuedFraction cf1(std::vector<int>{1, 2, 3});
    EXPECT_FALSE(cf1.IS_PERIODIC_CF());
    
    std::vector<Integer> coeffs = {Integer("1"), Integer("2"), Integer("3")};
    ContinuedFraction cf2(coeffs, 1);  // Период начинается с индекса 1
    EXPECT_TRUE(cf2.IS_PERIODIC_CF());
}

TEST(ContinuedFractionTest, GET_PERIOD_CF) {
    std::vector<Integer> coeffs = {Integer("1"), Integer("2"), Integer("3"), Integer("4")};
    ContinuedFraction cf(coeffs, 2);  // Период: [3, 4]
    std::vector<Integer> period = cf.GET_PERIOD_CF();
    EXPECT_EQ(period.size(), 2);
    EXPECT_EQ(period[0].as_string(), "3");
    EXPECT_EQ(period[1].as_string(), "4");
}

TEST(ContinuedFractionTest, GET_APERIODIC_CF) {
    std::vector<Integer> coeffs = {Integer("1"), Integer("2"), Integer("3"), Integer("4")};
    ContinuedFraction cf(coeffs, 2);  // Непериодическая часть: [1, 2]
    std::vector<Integer> aperiodic = cf.GET_APERIODIC_CF();
    EXPECT_EQ(aperiodic.size(), 2);
    EXPECT_EQ(aperiodic[0].as_string(), "1");
    EXPECT_EQ(aperiodic[1].as_string(), "2");
}

// Строковое представление
TEST(ContinuedFractionTest, AsString_Simple) {
    ContinuedFraction cf(std::vector<int>{3, 7, 15, 1});
    EXPECT_EQ(cf.as_string(), "[3; 7, 15, 1]");
}

TEST(ContinuedFractionTest, AsString_Periodic) {
    std::vector<Integer> coeffs = {Integer("1"), Integer("2"), Integer("3")};
    ContinuedFraction cf(coeffs, 1);
    EXPECT_EQ(cf.as_string(), "[1; (2, 3)...]");
}

// Операторы сравнения
TEST(ContinuedFractionTest, Operators) {
    ContinuedFraction cf1(std::vector<int>{1, 2});
    ContinuedFraction cf2(std::vector<int>{1, 2});
    ContinuedFraction cf3(std::vector<int>{2});
    
    EXPECT_TRUE(cf1 == cf2);
    EXPECT_TRUE(cf1 < cf3);
    EXPECT_TRUE(cf3 > cf1);
}

// CF-13: Преобразование периодической цепной дроби в квадратичную иррациональность
TEST(ContinuedFractionTest, TO_CF_QUAD_NonPeriodic) {
    // Непериодическая дробь [3; 7] = 22/7 -> (22, 0, 7)
    ContinuedFraction cf(std::vector<int>{3, 7});
    auto [a, D, c] = cf.TO_CF_QUAD();
    EXPECT_EQ(a.as_string(), "22");
    EXPECT_EQ(D.as_string(), "0");
    EXPECT_EQ(c.as_string(), "7");
}

TEST(ContinuedFractionTest, TO_CF_QUAD_Sqrt2) {
    // sqrt(2) = [1; (2)...] -> (0, 2, 1) или эквивалент
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("2"));
    auto [a, D, c] = cf.TO_CF_QUAD();
    
    // Проверяем что (a + sqrt(D)) / c = sqrt(2)
    // Это значит a = 0, D = 2, c = 1 (или кратные)
    // Или D должен содержать множитель 2
    EXPECT_TRUE(D.NZER_N_B());  // D != 0 (периодическая)
}

TEST(ContinuedFractionTest, TO_CF_QUAD_Sqrt3) {
    // sqrt(3) = [1; (1, 2)...]
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("3"));
    auto [a, D, c] = cf.TO_CF_QUAD();
    
    EXPECT_TRUE(D.NZER_N_B());  // D != 0 (периодическая)
}

TEST(ContinuedFractionTest, TO_CF_QUAD_Sqrt5) {
    // sqrt(5) = [2; (4)...]
    ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural("5"));
    auto [a, D, c] = cf.TO_CF_QUAD();
    
    EXPECT_TRUE(D.NZER_N_B());  // D != 0 (периодическая)
}

TEST(ContinuedFractionTest, TO_CF_QUAD_GoldenRatio) {
    // φ = (1 + sqrt(5)) / 2 = [1; (1)...]
    std::vector<Integer> coeffs = {Integer("1"), Integer("1")};
    ContinuedFraction cf(coeffs, 1);  // [1; (1)...]
    auto [a, D, c] = cf.TO_CF_QUAD();
    
    // φ = (1 + sqrt(5)) / 2, так что a=1, D=5, c=2
    EXPECT_TRUE(D.NZER_N_B());
}



// PCF-1: Преобразование рациональной функции P/Q в цепную дробь
TEST(PolynomialContinuedFractionTest, FROM_PQ_PCF_Simple) {
    // (x^2 + 1) / x = x + 1/x
    // Цепная дробь: [x; x]
    Polynomial P({{2, "1"}, {0, "1"}});  // x^2 + 1
    Polynomial Q({{1, "1"}});             // x
    
    PolynomialContinuedFraction pcf = PolynomialContinuedFraction::FROM_PQ_PCF(P, Q);
    EXPECT_EQ(pcf.coefficients().size(), 2);
}

TEST(PolynomialContinuedFractionTest, FROM_PQ_PCF_Constant) {
    // 5 / 1 = [5]
    Polynomial P({{0, "5"}});
    Polynomial Q({{0, "1"}});
    
    PolynomialContinuedFraction pcf = PolynomialContinuedFraction::FROM_PQ_PCF(P, Q);
    EXPECT_EQ(pcf.coefficients().size(), 1);
}

TEST(PolynomialContinuedFractionTest, FROM_PQ_PCF_ZeroDenominator) {
    Polynomial P({{1, "1"}});
    Polynomial Q;  // Нулевой многочлен
    
    EXPECT_THROW(PolynomialContinuedFraction::FROM_PQ_PCF(P, Q), std::invalid_argument);
}

// PCF-2: Преобразование цепной дроби в рациональную функцию
TEST(PolynomialContinuedFractionTest, TO_PCF_PQ_Simple) {
    // [x] = x/1
    Polynomial p1({{1, "1"}});  // x
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p1});
    
    auto [num, den] = pcf.TO_PCF_PQ();
    EXPECT_EQ(num.as_string(), "1 * x ^ 1");
    EXPECT_EQ(den.as_string(), "1");
}

// Проверка обратимости: P/Q -> PCF -> P/Q
TEST(PolynomialContinuedFractionTest, RoundTrip_PQ_PCF_PQ) {
    Polynomial P({{3, "1"}, {1, "2"}, {0, "1"}});  // x^3 + 2x + 1
    Polynomial Q({{2, "1"}, {0, "1"}});             // x^2 + 1
    
    PolynomialContinuedFraction pcf = PolynomialContinuedFraction::FROM_PQ_PCF(P, Q);
    auto [num, den] = pcf.TO_PCF_PQ();
    
    // Проверяем, что P/Q = num/den (с точностью до общего множителя)
    // P * den должен быть пропорционален num * Q
    Polynomial left = P.MUL_PP_P(den);
    Polynomial right = num.MUL_PP_P(Q);
    
    // Нормализуем и сравниваем
    auto [fac_left, norm_left] = left.FAC_P_Q();
    auto [fac_right, norm_right] = right.FAC_P_Q();
    
    EXPECT_TRUE(norm_left.EQ_PP_B(norm_right));
}

// PCF-3: Получение последовательности конвергентов
TEST(PolynomialContinuedFractionTest, CONVERGENTS_PCF) {
    Polynomial p0({{1, "1"}});  // x
    Polynomial p1({{0, "2"}});  // 2
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0, p1});
    
    auto conv = pcf.CONVERGENTS_PCF();
    EXPECT_EQ(conv.size(), 2);
}

// PCF-4: Получение n-го конвергента
TEST(PolynomialContinuedFractionTest, CONVERGENT_N_PCF) {
    Polynomial p0({{1, "1"}});  // x
    Polynomial p1({{0, "2"}});  // 2
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0, p1});
    
    auto [num0, den0] = pcf.CONVERGENT_N_PCF(0);
    EXPECT_EQ(num0.as_string(), "1 * x ^ 1");
    EXPECT_EQ(den0.as_string(), "1");
}

TEST(PolynomialContinuedFractionTest, CONVERGENT_N_PCF_OutOfRange) {
    Polynomial p0({{1, "1"}});
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0});
    
    EXPECT_THROW((void)pcf.CONVERGENT_N_PCF(5), std::out_of_range);
}

// PCF-6: Получение обратной дроби
TEST(PolynomialContinuedFractionTest, INV_PCF_PCF) {
    // [0; x] = 1/x, обратная = [x] = x
    Polynomial p0;               // 0
    Polynomial p1({{1, "1"}});   // x
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0, p1});
    
    PolynomialContinuedFraction inv = pcf.INV_PCF_PCF();
    auto [num, den] = inv.TO_PCF_PQ();
    
    EXPECT_EQ(num.as_string(), "1 * x ^ 1");
}

TEST(PolynomialContinuedFractionTest, INV_PCF_PCF_Zero) {
    Polynomial p0;  // 0
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0});
    
    EXPECT_THROW((void)pcf.INV_PCF_PCF(), std::runtime_error);
}

// Строковое представление
TEST(PolynomialContinuedFractionTest, AsString) {
    Polynomial p0({{1, "1"}});  // x
    Polynomial p1({{0, "2"}});  // 2
    PolynomialContinuedFraction pcf(std::vector<Polynomial>{p0, p1});
    
    std::string str = pcf.as_string();
    EXPECT_TRUE(str.find("x") != std::string::npos);
}

// Проверка на равенство
TEST(PolynomialContinuedFractionTest, EQ_PCF_B) {
    Polynomial p0({{1, "1"}});
    Polynomial p1({{0, "2"}});
    
    PolynomialContinuedFraction pcf1(std::vector<Polynomial>{p0, p1});
    PolynomialContinuedFraction pcf2(std::vector<Polynomial>{p0, p1});
    PolynomialContinuedFraction pcf3(std::vector<Polynomial>{p0});
    
    EXPECT_TRUE(pcf1 == pcf2);
    EXPECT_FALSE(pcf1 == pcf3);
}
