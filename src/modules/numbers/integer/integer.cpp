#include "integer.hpp"

Integer::Integer() : Natural(), s_(false) {}

Integer::Integer(std::string number):
    s_(number[0] == '-'),
    Natural(number[0] == '-' ? number.substr(1) : number) 
{}

Integer::Integer(bool s, std::string number):
    s_(s),
    Natural(number)
{}

std::string Integer::as_string() const {
    std::string str = a_;

    if (s_) str.push_back('-');

    std::reverse(str.begin(), str.end());

    return str;
}

//Z-1
Natural Integer::ABS_Z_N() const {
    std::string result = as_string();
    if (s_)
        result.erase(0, 1);
    return Natural(result);
}

//Z-2
int Integer::SGN_Z_D() const{
    if(a_[n_] == '0') return 0;
    if(s_) return -1;
    return 1;
}

//Z-3
Integer Integer::MUL_ZM_Z() const{
    Integer result = *this;
    result.s_ = !result.s_; //просто меняем знак
    return result;
}

//Z-4
Integer Integer::TRANS_N_Z(const Natural& natural){ 
    return Integer(natural.as_string());
}

//Z-5
Natural Integer::TRANS_Z_N() const{
    std::string from = a_;

    reverse(from.begin(), from.end());

    return Natural(from);
}

//Z-6
Integer Integer::ADD_ZZ_Z(const Integer& other) const{
    if (this->SGN_Z_D() != other.SGN_Z_D()) {
        if (this->SGN_Z_D() == 1)
            return this->SUB_ZZ_Z(other.MUL_ZM_Z());
        else
            return other.SUB_ZZ_Z(this->MUL_ZM_Z());
    }

    Natural summand1 = this->TRANS_Z_N();
    Natural summand2 = other.TRANS_Z_N();
    Natural sum = summand1.ADD_NN_N(summand2);

    Integer result = result.TRANS_N_Z(sum);
    if (this->SGN_Z_D() == -1)
        result = result.MUL_ZM_Z();

    return result;
}

//Z-7
Integer Integer::SUB_ZZ_Z(const Integer& other) const{
    Natural reduced = this->ABS_Z_N();
    Natural subtrahend = other.ABS_Z_N();
    bool switch_sign = false;
    if (reduced.COM_NN_D(subtrahend) == 1){
        std::swap(reduced, subtrahend);
        switch_sign = true;
    }

    Natural sub;

    if (this->SGN_Z_D() != other.SGN_Z_D()){
        sub = reduced.ADD_NN_N(subtrahend);
        if (this->SGN_Z_D() == -1)
            switch_sign = !switch_sign;
    } else {
        sub = reduced.SUB_NN_N(subtrahend);
        if (this->SGN_Z_D() == -1)
            switch_sign = !switch_sign;
    }

    Integer result = result.TRANS_N_Z(sub);
    if (switch_sign)
        result = result.MUL_ZM_Z();

    return result;
}

//Z-8
Integer Integer::MUL_ZZ_Z(const Integer& other) const{
    Natural mul1 = this->ABS_Z_N();
    Natural mul2 = other.ABS_Z_N();
    Natural mul = mul1.MUL_NN_N(mul2);

    Integer result = result.TRANS_N_Z(mul);
    if (this->SGN_Z_D() != other.SGN_Z_D())
        result = result.MUL_ZM_Z();

    return result;
}

//Z-9
Integer Integer::DIV_ZZ_Z(const Integer& other) const {
    Natural a = this->ABS_Z_N();
    Natural b = other.ABS_Z_N();
    Natural div = a.DIV_NN_N(b);

    Integer result = result.TRANS_N_Z(div);
    if (this->SGN_Z_D() != other.SGN_Z_D()) {
        result = result.ADD_ZZ_Z(Integer("1"));
        result = result.MUL_ZM_Z();
    }

    return result;
}

//Z-10
Integer Integer::MOD_ZZ_Z(const Integer& other) const {
    if (this->SGN_Z_D() == other.SGN_Z_D()) {
        Natural divisible = this->ABS_Z_N();
        Natural divisor = other.ABS_Z_N();
        Natural div = divisible.MOD_NN_N(divisor);

        Integer result = result.TRANS_N_Z(div);
        return result;
    }

    Integer div = this->DIV_ZZ_Z(other);

    Integer closest_max = div.MUL_ZZ_Z(other);
    if (closest_max.SGN_Z_D() == -1)
        closest_max = closest_max.MUL_ZM_Z();

    Integer divisible = *this;
    if (divisible.SGN_Z_D() == -1)
        divisible = divisible.MUL_ZM_Z();

    Integer result = closest_max.SUB_ZZ_Z(divisible);

    return result;
}

std::ostream &operator<<(std::ostream &os, const Integer &number) {
    return os << number.as_string();
}