#pragma once

#include "rational.hpp"

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
     *  с_[0] - коэфициент при x^0
     *  с_[m] - коэфициент при x^m
     */
    std::vector<Rational> c_;

public:
    // конструктор по-умолчанию
    Polynomial();

    // конструктор, создает нулевой полином степени deg
    Polynomial(int deg);

    // конструктор, создает полином из набора коэффициентов и степеней к ним
    Polynomial(std::vector<std::string>& coeffs, std::vector<int> degs);

    // строковое представление полинома
    std::string as_string() const;

    // проверка двух подиномов на идентичность
    bool EQ_PP_B(const Polynomial& other) const;

    // проверка полинома на ноль
    bool NZER_P_B() const;

    // P-1 | Сложение многочленов
    Polynomial ADD_PP_P(const Polynomial& other) const;
    
    // P-2 | Вычитание многочленов
    Polynomial SUB_PP_P(const Polynomial& other) const;
    
    // P-3 | Умножение многочлена на рациональное число
    Polynomial MUL_PQ_P(const Rational& rational) const;
    
    // P-4 | Умножение многочлена на x^k, k-натуральное или 0
    Polynomial MUL_Pxk_P(int k) const;
    
    // P-5 | Старший коэффициент многочлена
    Rational LED_P_Q() const;
    
    // P-6 | Степень многочлена
    Natural DEG_P_N() const;
    
    // P-7 | Вынесение из многочлена НОК знаменателей коэффициентов и НОД числителей
    std::pair<Rational, Polynomial> FAC_P_Q() const;
    
    // P-8 | Умножение многочленов
    Polynomial MUL_PP_P(const Polynomial& other) const;
    
    // P-9 |Частное от деления многочлена на многочлен при делении с остатком
    Polynomial DIV_PP_P(const Polynomial& other) const;
    
    // P-10 | Остаток от деления многочлена на многочлен при делении с остатком
    Polynomial MOD_PP_P(const Polynomial& other) const;
    
    // P-11 | НОД многочленов
    Polynomial GCF_PP_P(const Polynomial& other) const;
    
    // P-12 | Производная многочлена
    Polynomial DER_P_P() const;
    
    // P-13 | Преобразование многочлена — кратные корни в простые
    Polynomial NMR_P_P() const;

    // нормирование полинома
    Polynomial Polynomial::NORM_P_P() const;
};

std::ostream& operator << (std::ostream &os, const Polynomial& p);
