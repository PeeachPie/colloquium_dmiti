#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

/*
 *   класс натуральных чисел с нулем
 */
class Natural {
protected:
    /*
     *  строка цифр числа
     *   
     *  a[0] - младшая цифра
     *  a[n] - старшая цифра
     */
    std::string a_;

    // старший разряд числа
    int n_;

public:
    // Конструктор по умолчанию создает число равное 0
    Natural();

    // Превращает строку в число
    // Ведущие нули обрезаются, число сохраняется в перевернутом виде
    Natural(std::string number);

    std::string as_string() const;

    // N-1 | Сравнение натуральных чисел: 2 - если первое больше второго, 0, если равно, 1 иначе.
    int COM_NN_D(const Natural& other) const;
    
    // N-2 | Проверка на ноль: если число не равно нулю, то true иначе false
    bool NZER_N_B() const;
    
    // N-3 | Добавление 1 к натуральному числу
    Natural ADD_1N_N();
    
    // N-4 | Сложение натуральных чисел
    // (!) в зависимостях указан модуль COM_NN_D - не уверен, что он тут нужен
    Natural ADD_NN_N(const Natural& other) const;
    
    // N-5 | Вычитание из первого большего натурального числа второго меньшего или равного
    // Если второе число больше первого - выбрасывается ошибка
    Natural SUB_NN_N(const Natural& other) const;
    
    // N-6 | Умножение натурального числа на цифру
    Natural MUL_ND_N(int digit) const;
    
    // N-7 | Умножение натурального числа на 10^k, k-натуральное (не длинное)
    Natural MUL_Nk_N(int k) const;
    
    // N-8 | Умножение натуральных чисел в столбик
    Natural MUL_NN_N(const Natural& other) const;
    
    // N-9 | Вычитание из натурального другого натурального, умноженного на цифру для случая с неотрицательным результатом
    Natural SUB_NDN_N(const Natural& other, int digit) const;
    
    // N-10 | Вычисление первой цифры деления большего натурального на меньшее, домноженное на 10^k, где k - номер позиции этой цифры (номер считается с нуля)
    std::pair<int, int> DIV_NN_Dk(const Natural& other) const;
    
    // N-11 | Неполное частное от деления первого натурального числа на второе
    // Если делитель равен нулю - выбрасывается ошибка
    Natural DIV_NN_N(const Natural& other) const;
    
    // N-12 | Остаток от деления первого натурального числа на второе натуральное (делитель отличен от нуля)
    Natural MOD_NN_N(const Natural& other) const;
    
    // N-13 | НОД натуральных чисел
    Natural GCF_NN_N(const Natural& other) const;
    
    // N-14 | НОК натуральных чисел
    Natural LCM_NN_N(const Natural& other) const;
};

// Для удобного вывода в консоль
std::ostream& operator << (std::ostream &os, const Natural &number);

bool operator == (const Natural &number1, const Natural &number2);
bool operator != (const Natural &number1, const Natural &number2);