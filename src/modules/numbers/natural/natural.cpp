#include "natural.hpp"

Natural::Natural(): n_(0), a_({'0'}) {};

Natural::Natural(std::string s) {
    int first_not_zero = 0;

    // находим первую не нулевую цифру, либо единственный ноль
    while (first_not_zero < s.size() && s[first_not_zero] == '0') {
        first_not_zero++;
    }

    // сохраняем массив цифр
    for (int i = first_not_zero; i < s.size(); i++) {
        if (isdigit(s[i])) {
            a_.push_back(s[i]);
        }
        else {
            throw std::invalid_argument("Строка должна содержать только цифры!");
        }
    }

    // для удобства вычислений переворачиваем

    std::reverse(a_.begin(), a_.end());

    n_ = a_.size();
};

std::string Natural::as_string() const {
    std::string str = a_;

    std::reverse(str.begin(), str.end());

    return str;
}

std::ostream& operator << (std::ostream &os, const Natural &number) {
    return os << number.as_string();
}