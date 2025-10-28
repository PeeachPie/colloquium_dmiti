#include "integer.hpp"

Integer::Integer() : Natural(), s_(false) {}
Integer::Integer(std::string number){
    if (number != "" && number[0] == '-') {
        s_ = true;
        number = number.substr(1);
    } else {
        s_ = false;
    }
        
    Natural tmp(number);
    this->a_ = tmp.a_;
    this->n_ = tmp.n_;
        
    //пусть знак нуля будет отрицательным
    if (a_ == "0") {
        s_ = true;
    } 
}

//Z-1
Natural Integer::ABS_Z_N() const {
    std::string result = as_string();
    return Natural(result);
}

//Z-2
int Integer::SGN_Z_D() const{
    if(n_ == 0 && a_[0] == '0') return 0;
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
    Integer result;
    result.n_ = natural.n_;
    result.a_ = natural.a_;
    result.s_ = false;

    return result;
}

//Z-5
Natural Integer::TRANS_Z_N() const{
    Natural result;
    result.n_ = n_;
    result.a_ = a_;

    return result;
}

//Z-6
Integer Integer::ADD_ZZ_Z(const Integer& other) const{
    Integer result;
    Natural summand1 = TRANS_Z_N();
    Natural summand2 = other.TRANS_Z_N();

    if( (s_ && other.s_) || (!s_ && !other.s_)){ //оба одного знака
        result = summand1.ADD_NN_N(summand2);
        result.s_ = s_;
        return result;
    }
    
    //из большего вычитаем меньшее
    if(summand1.COM_NN_D(summand2) == 1){
        std::swap(summand1, summand2);
        result.s_ = false;
    } else {
        result.s_ = true;
    }

    Natural tmp = summand1.SUB_NN_N(summand2);
    result.a_ = tmp.a_; result.n_ = tmp.n_;

    return result;
}

//Z-7
Integer Integer::SUB_ZZ_Z(const Integer& other) const{
    Integer result;
    Integer reduced = *this;
    Integer subtrahend = other;

    if(s_ && !other.s_){ //-a и b -> -a - b (уберем знаки сложим вернем знак)
        reduced.s_ = false; subtrahend.s_ = false; //убираем знаки
    } 

    else if(!s_ && !other.s_){ //a и b -> a - b
        subtrahend.s_ = true; //добавляем знак
    } 

    else { //-a и -b -> -a + b; a и -b -> a + b
        subtrahend.s_ = false; //убираем знак 
    }
    
    result = reduced.ADD_ZZ_Z(subtrahend); //складываем
    if(s_ && !other.s_) result.s_ = true; //возвращаем знак
    return result;
}

//Z-8
Integer Integer::MUL_ZZ_Z(const Integer& other) const{
    Integer result;
    Natural mul1 = TRANS_Z_N();
    Natural mul2 = other.TRANS_Z_N();
    
    Natural mul = mul1.MUL_NN_N(mul2);
    result.n_ = mul.n_; result.a_ = mul.a_;

    if((!s_ && !other.s_) || (s_ && result.s_)) {
        result.s_ = false;
    }
    else{
        result.s_ = true;
    }

    return result;
}

