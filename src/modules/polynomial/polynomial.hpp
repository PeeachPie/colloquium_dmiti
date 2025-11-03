#pragma once

#include "rational.hpp"

#include <iostream>
#include <string>
#include <map>

/*
 *   класс многочленов над рациональными числами
 */
class Polynomial {
private:
    // степень многочлена
    int m_;

    /*
     *  массив коэфициентов полинома
     *   
     *  с_[i] - коэфициент при x^i
     */
    std::map<int, Rational, std::greater<int>> c_;

public:
    // конструктор по-умолчанию
    Polynomial();

    // конструктор, создает нулевой полином степени deg
    // Polynomial(int deg);

    // конструктор, создает полином из набора коэффициентов и степеней к ним
    Polynomial(std::vector<std::pair<int, std::string>> polynomial);

    // строковое представление полинома
    [[nodiscard]] std::string as_string() const;

    // проверка двух полиномов на идентичность
    [[nodiscard]] bool EQ_PP_B(const Polynomial& other) const;

    // проверка полинома на ноль
    [[nodiscard]] bool NZER_P_B() const;

    // P-1 | Сложение многочленов
    [[nodiscard]] Polynomial ADD_PP_P(const Polynomial& other) const;
    
    // P-2 | Вычитание многочленов
    [[nodiscard]] Polynomial SUB_PP_P(const Polynomial& other) const;
    
    // P-3 | Умножение многочлена на рациональное число
    [[nodiscard]] Polynomial MUL_PQ_P(const Rational& rational) const;
    
    // P-4 | Умножение многочлена на x^k, k-натуральное или 0
    [[nodiscard]] Polynomial MUL_Pxk_P(int k) const;
    
    // P-5 | Старший коэффициент многочлена
    [[nodiscard]] Rational LED_P_Q() const;
    
    // P-6 | Степень многочлена
    [[nodiscard]] Natural DEG_P_N() const;
    
    // P-7 | Вынесение из многочлена НОК знаменателей коэффициентов и НОД числителей
    [[nodiscard]] std::pair<Rational, Polynomial> FAC_P_Q() const;
    
    // P-8 | Умножение многочленов
    [[nodiscard]] Polynomial MUL_PP_P(const Polynomial& other) const;
    
    // P-9 |Частное от деления многочлена на многочлен при делении с остатком
    [[nodiscard]] Polynomial DIV_PP_P(const Polynomial& other) const;
    
    // P-10 | Остаток от деления многочлена на многочлен при делении с остатком
    [[nodiscard]] Polynomial MOD_PP_P(const Polynomial& other) const;
    
    // P-11 | НОД многочленов
    [[nodiscard]] Polynomial GCF_PP_P(const Polynomial& other) const;
    
    // P-12 | Производная многочлена
    [[nodiscard]] Polynomial DER_P_P() const;
    
    // P-13 | Преобразование многочлена — кратные корни в простые
    [[nodiscard]] Polynomial NMR_P_P() const;

    // нормирование полинома
    [[nodiscard]] Polynomial NORM_P_P() const;
};

std::ostream& operator << (std::ostream &os, const Polynomial& p);

bool operator==(const Polynomial& p1, const Polynomial& p2);
