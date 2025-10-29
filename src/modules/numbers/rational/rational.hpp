#pragma once

#include "natural.hpp"
#include "integer.hpp"

/*
 *   класс рациональных чисел
 */
class Rational {
private:
    // числитель (целое число)
    Integer p_;
    // знаменатель (натуральное число)
    Natural q_;

public:
    // Q-1 | Сокращение дроби
    Rational& RED_Q_Q();
    
    // Q-2 | Проверка сокращенного дробного на целое, если рациональное число является целым, то «да», иначе «нет»
    [[nodiscard]] bool INT_Q_B() const;
    
    // Q-3 | Преобразование целого в дробное
    [[nodiscard]] static Rational TRANS_Z_Q(const Integer& integer);
    
    // Q-4 | Преобразование сокращенного дробного в целое (если знаменатель равен 1)
    [[nodiscard]] Integer TRANS_Q_Z() const;
    
    // Q-5 | Сложение дробей
    [[nodiscard]] Rational ADD_QQ_Q(const Rational& other) const;
    
    // Q-6 | Вычитание дробей
    [[nodiscard]] Rational SUB_QQ_Q(const Rational& other) const;
    
    // Q-7 | Умножение дробей
    [[nodiscard]] Rational MUL_QQ_Q(const Rational& other) const;
    
    // Q-8 | Деление дробей (делитель отличен от нуля)
    [[nodiscard]] Rational DIV_QQ_Q(const Rational& other) const;
};
