#include "integer.hpp"
#include <stdexcept>

Integer::Integer() : Natural(), s_(false) {}

Integer::Integer(std::string number) : s_(false), Natural("") {
    if (number.empty()) {
        throw std::invalid_argument("Строка должна содержать число");
    }

    bool negative = false;
    if (number[0] == '+' || number[0] == '-') {
        negative = (number[0] == '-');
        number.erase(number.begin());
    }

    Natural tmp(number);

    this->a_ = tmp.as_string();
    std::reverse(this->a_.begin(), this->a_.end());
    this->n_ = static_cast<int>(this->a_.size());

    s_ = (tmp.as_string() != "0") && negative;
}

std::string Integer::as_string() const {
    std::string abs_str = Natural::as_string();
    if (abs_str == "0") {
        return abs_str;
    }
    return (s_ ? std::string("-") + abs_str : abs_str);
}

Natural Integer::ABS_Z_N() const {
    return Natural(Natural::as_string());
}

int Integer::POZ_Z_D() const {
    std::string abs_str = Natural::as_string();
    if (abs_str == "0") return 0;
    return s_ ? 1 : 2;
}

Integer& Integer::MUL_ZM_Z() {
    if (Natural::as_string() != "0") {
        s_ = !s_;
    }
    return *this;
}

Integer Integer::TRANS_N_Z(const Natural& natural) {
    return Integer(natural.as_string());
}

Natural Integer::TRANS_Z_N() const {
    if (POZ_Z_D() == 1) {
        throw std::invalid_argument("Отрицательное целое нельзя преобразовать в натуральное");
    }
    return Natural(Natural::as_string());
}

Integer Integer::ADD_ZZ_Z(const Integer& /*other*/) const {
    throw std::logic_error("ADD_ZZ_Z не реализовано");
}

Integer Integer::SUB_ZZ_Z(const Integer& /*other*/) const {
    throw std::logic_error("SUB_ZZ_Z не реализовано");
}

Integer Integer::MUL_ZZ_Z(const Integer& /*other*/) const {
    throw std::logic_error("MUL_ZZ_Z не реализовано");
}

Integer Integer::DIV_ZZ_Z(const Integer& /*other*/) const {
    throw std::logic_error("DIV_ZZ_Z не реализовано");
}

Integer Integer::MOD_ZZ_Z(const Integer& /*other*/) const {
    throw std::logic_error("MOD_ZZ_Z не реализовано");
}
