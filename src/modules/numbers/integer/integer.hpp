#pragma once

#include "natural.hpp"

/*
 *   класс целых чисел
 */
class Integer : public Natural {
private:
    // знак числа (true — минус, false — плюс)
    bool s_;

public:
    
    Integer();

    Integer(std::string number);

    std::string as_string() const;

    // Z-1 Абсолютная величина числа, результат - натуральное
    Natural ABS_Z_N() const;
    
    // Z-2 Определение положительности числа (2 - положительное, 0 — равное нулю, 1 - отрицательное)
    // замена на предложенный вариант SGN_Z_D (1 - положительное, 0 - равное нулю,  -1 - отрицательное)
    //int POZ_Z_D() const;
    int SGN_Z_D() const;
    
    // Z-3 Умножение целого на (-1)
    // (?) тут опечатка с Integer& ?? я заменила на Integer, и добавила const
    Integer MUL_ZM_Z() const; 
    
    // Z-4 Преобразование натурального в целое
    Integer TRANS_N_Z(const Natural& natural);
    
    // Z-5 Преобразование целого неотрицательного в натуральное
    Natural TRANS_Z_N() const;
    
    // Z-6 Сложение целых чисел
    Integer ADD_ZZ_Z(const Integer& other) const;
    
    // Z-7 Вычитание целых чисел
    Integer SUB_ZZ_Z(const Integer& other) const;
    
    // Z-8 Умножение целых чисел
    Integer MUL_ZZ_Z(const Integer& other) const;
    
    // Z-9 Частное от деления целого на целое (делитель отличен от нуля)
    Integer DIV_ZZ_Z(const Integer& other) const;
    
    // Z-10 Остаток от деления целого на целое(делитель отличен от нуля)
    Integer MOD_ZZ_Z(const Integer& other) const;
};

std::ostream &operator<<(std::ostream &os, const Integer &number);