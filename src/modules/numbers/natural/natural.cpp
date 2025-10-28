#include "natural.hpp"

Natural::Natural() : n_(0), a_({'0'}) {};

Natural::Natural(std::string s)
{
    int first_not_zero = 0;

    // находим первую не нулевую цифру, либо единственный ноль
    while (first_not_zero < s.size() && s[first_not_zero] == '0')
    {
        first_not_zero++;
    }

    // сохраняем массив цифр
    for (int i = first_not_zero; i < s.size(); i++)
    {
        if (isdigit(s[i]))
        {
            a_.push_back(s[i]);
        }
        else
        {
            throw std::invalid_argument("Строка должна содержать только цифры!");
        }
    }

    // для удобства вычислений переворачиваем

    std::reverse(a_.begin(), a_.end());

    n_ = a_.size();
};

std::string Natural::as_string() const
{
    std::string str = a_;

    std::reverse(str.begin(), str.end());

    return str;
}

bool Natural::NZER_N_B() const
{
    return this->a_[n_] - '0';
}

Natural Natural::MUL_NN_N(const Natural &other) const
{
    Natural result;

    for (int i = 0; i < other.n_; i++)
    {
        // переменная, которая будет содержать результат умножения
        // числа на текущую цифру другого числа с поправкой на разряд цифры
        Natural step = this->MUL_ND_N(other.a_[i] - '0'); // умножаем число на текущую цифру другого числа

        // домножаем на 10^i
        step = step.MUL_Nk_N(i);

        // складываем результат, полученный на текущем шаге, с общим результатом
        result = result.ADD_NN_N(step);
    }

    return result;
}

Natural Natural::DIV_NN_N(const Natural &other) const
{
    // если делитель не равен 0
    if (other.NZER_N_B())
    {
        // для сохранения неполного частного
        Natural result;
        // для хранения текущего делимого
        Natural current_num = *this;

        // пока текущее делимое не стане меньше делителя
        while (current_num.COM_NN_D(other) != 1)
        {
            // получаем первую цифру (digit) неполного частного от деления
            // текущего делимого на делитель и ее разряд (k)
            std::pair<int, int> digit_k = current_num.DIV_NN_Dk(other);

            // добавляем первую цифру неполного частного в результат
            result.a_.push_back(digit_k.first - '0');

            // возводим десять в степень k и вычитаем из текущего делимого,
            // домножив предварительно на digit
            Natural ranks = Natural("1").MUL_Nk_N(digit_k.second);
            current_num = current_num.SUB_NDN_N(ranks, digit_k.first);
        }

        // удаляем лишний 0 с конца результата
        result.a_.erase(0, 1);

        // изменяем размер результата на актуальный
        result.n_ = result.a_.size();

        return result;
    }
    // если делитель равен 0
    else
    {
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");
    }
}

Natural Natural::MOD_NN_N(const Natural &other) const
{
    // если делитель не равен 0
    if (other.NZER_N_B())
    {
        // для хранения текущего делимого
        Natural current_num = *this;

        // пока текущее делимое не стане меньше делителя
        while (current_num.COM_NN_D(other) != 1)
        {
            // получаем первую цифру (digit) неполного частного от деления
            // текущего делимого на делитель и ее разряд (k)
            std::pair<int, int> digit_k = current_num.DIV_NN_Dk(other);

            // возводим десять в степень k и вычитаем из текущего делимого,
            // домножив предварительно на digit
            Natural ranks = Natural("1").MUL_Nk_N(digit_k.second);
            current_num = current_num.SUB_NDN_N(ranks, digit_k.first);
        }

        // последнее значение текущего делимого есть остаток от деления
        return current_num;
    }
    // если делитель равен 0
    else
    {
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");
    }
}

Natural Natural::GCF_NN_N(const Natural& other) const {
    // Алгоритм Евклида для нахождения НОД

    // Начальные значения
    Natural a = *this;
    Natural b = other;

    // Пока a и b больше 0
    while (a.NZER_N_B() && b.NZER_N_B()) {
        // Берем остаток от деления большего на меньшее
        if (a.COM_NN_D(b) != 1)
            a = a.MOD_NN_N(b);
        else
            b = b.MOD_NN_N(a);
    }

    // После завершения цикла выбираем максимальное из двух оставшихся чисел
    Natural max;
    if (a.COM_NN_D(b) != 1)
        max = a;
    else
        max = b;

    return max;
}

Natural Natural::LCM_NN_N(const Natural& other) const {
    // связь НОД и НОК: (a * b) / GCD(a, b)
    Natural numerator = this->MUL_NN_N(other); // числитель дроби
    Natural denominator = this->GCF_NN_N(other); // знаменатель дроби

    return numerator.DIV_NN_N(denominator); // деление числителя на знаменатель
}

std::ostream &operator<<(std::ostream &os, const Natural &number)
{
    return os << number.as_string();
}