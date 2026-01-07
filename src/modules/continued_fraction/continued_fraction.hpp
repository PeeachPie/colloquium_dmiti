#pragma once

#include "rational.hpp"
#include "polynomial.hpp"
#include <vector>
#include <string>
#include <optional>
#include <tuple>

/*
 *   Класс цепных дробей для рациональных чисел
 *   
 *   Цепная дробь представляется как [a0; a1, a2, ..., an]
 *   где a0 - целая часть, a1..an - частичные частные
 */
class ContinuedFraction {
private:
    // Частичные частные цепной дроби [a0; a1, a2, ..., an]
    std::vector<Integer> coefficients_;
    
    // Для периодических цепных дробей (квадратичные иррациональности)
    // period_start_ указывает индекс начала периода (-1 если не периодическая)
    int period_start_;

public:
    // Конструктор по умолчанию: создаёт [0]
    ContinuedFraction();
    
    // Конструктор из вектора коэффициентов
    explicit ContinuedFraction(const std::vector<Integer>& coefficients);
    
    // Конструктор из вектора целых чисел (для удобства)
    explicit ContinuedFraction(const std::vector<int>& coefficients);
    
    // Конструктор для периодических цепных дробей
    ContinuedFraction(const std::vector<Integer>& coefficients, int period_start);

    // CF-1 | Преобразование рационального числа в цепную дробь
    static ContinuedFraction FROM_Q_CF(const Rational& rational);
    
    // CF-2 | Преобразование цепной дроби в рациональное число
    [[nodiscard]] Rational TO_CF_Q() const;
    
    // CF-3 | Получение последовательности конвергентов (подходящих дробей)
    [[nodiscard]] std::vector<Rational> CONVERGENTS_CF() const;
    
    // CF-4 | Получение n-го конвергента (подходящей дроби)
    [[nodiscard]] Rational CONVERGENT_N_CF(int n) const;
    
    // CF-5 | Получение приближения с заданной точностью (знаменатель <= max_denominator)
    [[nodiscard]] Rational APPROX_CF_Q(const Natural& max_denominator) const;
    
    // CF-6 | Сравнение двух цепных дробей
    // Возвращает: -1 если this < other, 0 если равны, 1 если this > other
    [[nodiscard]] int COM_CF_D(const ContinuedFraction& other) const;
    
    // CF-7 | Получение обратной дроби (переворот)
    [[nodiscard]] ContinuedFraction INV_CF_CF() const;
    
    // CF-8 | Преобразование квадратичной иррациональности sqrt(D) в периодическую цепную дробь
    // Возвращает цепную дробь для sqrt(D)
    static ContinuedFraction FROM_SQRT_CF(const Natural& D);
    
    // CF-9 | Преобразование (a + sqrt(D)) / c в периодическую цепную дробь
    // Общая форма квадратичной иррациональности
    static ContinuedFraction FROM_QUAD_CF(const Integer& a, const Natural& D, const Integer& c);
    
    // CF-10 | Проверка, является ли дробь периодической
    [[nodiscard]] bool IS_PERIODIC_CF() const;
    
    // CF-11 | Получение периода (для периодических дробей)
    [[nodiscard]] std::vector<Integer> GET_PERIOD_CF() const;
    
    // CF-12 | Получение непериодической части
    [[nodiscard]] std::vector<Integer> GET_APERIODIC_CF() const;
    
    // CF-13 | Преобразование периодической цепной дроби в квадратичную иррациональность
    // Возвращает (a, D, c) такие что дробь = (a + sqrt(D)) / c
    // Если дробь не периодическая, возвращает рациональное число как (p, 0, q) где p/q - значение
    [[nodiscard]] std::tuple<Integer, Natural, Integer> TO_CF_QUAD() const;

    // Геттеры
    [[nodiscard]] const std::vector<Integer>& coefficients() const { return coefficients_; }
    [[nodiscard]] int period_start() const { return period_start_; }
    [[nodiscard]] size_t length() const { return coefficients_.size(); }
    
    // Строковое представление
    [[nodiscard]] std::string as_string() const;
    
    // Проверка на равенство
    [[nodiscard]] bool EQ_CF_B(const ContinuedFraction& other) const;
};

std::ostream& operator<<(std::ostream& os, const ContinuedFraction& cf);
bool operator==(const ContinuedFraction& cf1, const ContinuedFraction& cf2);
bool operator<(const ContinuedFraction& cf1, const ContinuedFraction& cf2);
bool operator>(const ContinuedFraction& cf1, const ContinuedFraction& cf2);


/*
 *   Класс цепных дробей для многочленов
 *   
 *   Цепная дробь многочленов: [P0; P1, P2, ..., Pn]
 *   где Pi - многочлены
 */
class PolynomialContinuedFraction {
private:
    std::vector<Polynomial> coefficients_;

public:
    // Конструктор по умолчанию
    PolynomialContinuedFraction();
    
    // Конструктор из вектора многочленов
    explicit PolynomialContinuedFraction(const std::vector<Polynomial>& coefficients);

    // PCF-1 | Преобразование рациональной функции P/Q в цепную дробь
    static PolynomialContinuedFraction FROM_PQ_PCF(const Polynomial& P, const Polynomial& Q);
    
    // PCF-2 | Преобразование цепной дроби в рациональную функцию (числитель, знаменатель)
    [[nodiscard]] std::pair<Polynomial, Polynomial> TO_PCF_PQ() const;
    
    // PCF-3 | Получение последовательности конвергентов
    [[nodiscard]] std::vector<std::pair<Polynomial, Polynomial>> CONVERGENTS_PCF() const;
    
    // PCF-4 | Получение n-го конвергента
    [[nodiscard]] std::pair<Polynomial, Polynomial> CONVERGENT_N_PCF(int n) const;
    
    // PCF-5 | Сравнение двух цепных дробей многочленов (по степени)
    [[nodiscard]] int COM_PCF_D(const PolynomialContinuedFraction& other) const;
    
    // PCF-6 | Получение обратной дроби
    [[nodiscard]] PolynomialContinuedFraction INV_PCF_PCF() const;

    // Геттеры
    [[nodiscard]] const std::vector<Polynomial>& coefficients() const { return coefficients_; }
    [[nodiscard]] size_t length() const { return coefficients_.size(); }
    
    // Строковое представление
    [[nodiscard]] std::string as_string() const;
    
    // Проверка на равенство
    [[nodiscard]] bool EQ_PCF_B(const PolynomialContinuedFraction& other) const;
};

std::ostream& operator<<(std::ostream& os, const PolynomialContinuedFraction& pcf);
bool operator==(const PolynomialContinuedFraction& pcf1, const PolynomialContinuedFraction& pcf2);
