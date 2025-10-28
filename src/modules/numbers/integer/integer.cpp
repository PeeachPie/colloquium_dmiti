#include "integer.hpp"
#include <stdexcept>
#include <algorithm>

 

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

Integer Integer::ADD_ZZ_Z(const Integer& other) const {
    Natural abs_this = this->ABS_Z_N();
    Natural abs_other = other.ABS_Z_N();
    bool sa = this->s_;
    bool sb = other.s_;

    if (abs_this.as_string() == "0") {
        return Integer(other.as_string());
    }
    if (abs_other.as_string() == "0") {
        return Integer(this->as_string());
    }

    Natural res_abs;
    bool res_neg = false;
    if (sa == sb) {
        res_abs = abs_this.ADD_NN_N(abs_other);
        res_neg = sa;
    } else {
        int cmp = abs_this.COM_NN_D(abs_other);
        if (cmp == 0) {
            return Integer("0");
        } else if (cmp == 2) {
            res_abs = abs_this.SUB_NN_N(abs_other);
            res_neg = sa;
        } else {
            res_abs = abs_other.SUB_NN_N(abs_this);
            res_neg = sb;
        }
    }
    if (res_abs.as_string() == "0") return Integer("0");
    return Integer((res_neg ? std::string("-") : std::string("")) + res_abs.as_string());
}

Integer Integer::SUB_ZZ_Z(const Integer& other) const {
    Integer neg_other = Integer(other.as_string());
    if (neg_other.Natural::as_string() != "0") {
        neg_other.s_ = !other.s_;
    }
    return this->ADD_ZZ_Z(neg_other);
}

Integer Integer::MUL_ZZ_Z(const Integer& other) const {
    Natural abs_this = this->ABS_Z_N();
    Natural abs_other = other.ABS_Z_N();
    
    if (abs_this.as_string() == "0" || abs_other.as_string() == "0") {
        return Integer("0");
    }
    
    try {
        Natural prod = abs_this.MUL_NN_N(abs_other);
        bool neg = (this->s_ != other.s_);
        return Integer((neg ? std::string("-") : std::string("")) + prod.as_string());
    } catch (...) {
        throw std::logic_error("MUL_NN_N не реализован в Natural");
    }
}

Integer Integer::DIV_ZZ_Z(const Integer& other) const {
    Natural abs_this = this->ABS_Z_N();
    Natural abs_other = other.ABS_Z_N();
    
    if (abs_other.as_string() == "0") {
        throw std::invalid_argument("Деление на ноль");
    }
    if (abs_this.as_string() == "0") {
        return Integer("0");
    }
    
    try {
        Natural q0 = abs_this.DIV_NN_N(abs_other);
        bool signs_diff = (this->s_ != other.s_);
        
        if (!signs_diff) {
            return Integer((this->s_ ? std::string("-") : std::string("")) + q0.as_string());
        } else {
            Natural r0 = abs_this.MOD_NN_N(abs_other);
            if (r0.as_string() == "0") {
                return Integer(std::string("-") + q0.as_string());
            } else {
                Natural q_inc = q0.ADD_1N_N();
                return Integer(std::string("-") + q_inc.as_string());
            }
        }
    } catch (...) {
        throw std::logic_error("DIV_NN_N не реализован в Natural");
    }
}

Integer Integer::MOD_ZZ_Z(const Integer& other) const {
    Natural abs_this = this->ABS_Z_N();
    Natural abs_other = other.ABS_Z_N();
    
    if (abs_other.as_string() == "0") {
        throw std::invalid_argument("Деление на ноль");
    }
    if (abs_this.as_string() == "0") {
        return Integer("0");
    }
    
    try {
        Natural r0 = abs_this.MOD_NN_N(abs_other);
        bool signs_diff = (this->s_ != other.s_);
        
        if (!signs_diff) {
            return Integer(r0.as_string());
        } else {
            if (r0.as_string() == "0") {
                return Integer("0");
            } else {
                Natural rr = abs_other.SUB_NN_N(r0);
                return Integer(rr.as_string());
            }
        }
    } catch (...) {
        throw std::logic_error("MOD_NN_N не реализован в Natural");
    }
}
