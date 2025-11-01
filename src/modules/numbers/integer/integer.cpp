#include "integer.hpp"

Integer::Integer() : m_(), s_(false) {}

Integer::Integer(const std::string& number):
    s_(number[0] == '-'),
    m_(number[0] == '-' ? number.substr(1) : number) 
{}

Integer::Integer(bool s, const std::string& number):
    s_(s),
    m_(number)
{}

std::string Integer::as_string() const {
    // сохраняем поле Natural m_ как строку
    std::string str = m_.as_string();

    // если данное целое число отрицательное и не 0,
    // то вставляем в начало минус
    if (s_ && m_.NZER_N_B())
        str = '-' + str;

    return str;
}

//Z-0
int Integer::COM_ZZ_D(const Integer& other) const {
    // если числа одного знака
    if (s_ == other.s_) {
        // сравниваем модули, затем делаем поправку на знак
        if (m_.COM_NN_D(other.m_) == 2) {
            if (s_)
                return -1;
            else
                return 1;
        } else if (m_.COM_NN_D(other.m_) == 1) {
            if (s_)
                return 1;
            else
                return -1;
        } else
            return 0;
    // иначе больше то, которое положительно
    } else {
        if (s_)
            return -1;
        else
            return 1;
    }
}


//Z-1
Natural Integer::ABS_Z_N() const {
    return m_; // возвращаем содержимое поля Natural m_
}

//Z-2
int Integer::SGN_Z_D() const{
    // если число равно 0
    if(!m_.NZER_N_B()) return 0;
    // если число отрицательно
    if(s_) return -1;
    // если число положительно
    return 1;
}

//Z-3
Integer Integer::MUL_ZM_Z() const{
    // если 0, то ничего не делаем
    if (!m_.NZER_N_B())
        return *this;
    // иначе меняем знак
    Integer result = *this;
    result.s_ = !result.s_;
    return result;
}

//Z-4
Integer Integer::TRANS_N_Z(const Natural& natural){ 
    return Integer(natural.as_string()); // создаем целое число из заданного натурального
}

//Z-5
Natural Integer::TRANS_Z_N() const{
    // если число отрицательно и не 0, то выбрасывается исключение
    if (s_ && m_.NZER_N_B())
        throw std::invalid_argument("Число должно быть неотрицательным!");

    return m_; // возвращаем содержимое поля Natural m_
}

//Z-6
Integer Integer::ADD_ZZ_Z(const Integer& other) const{
    Integer result;

    // если оба слагаемых одного знака
    if (s_ == other.s_) {
        result = result.TRANS_N_Z(m_.ADD_NN_N(other.m_));
        result.s_ = s_;
    } else {
        // если слагаемые разных знаков и ->
        // знак берем от большего по модулю числа
        if (m_.COM_NN_D(other.m_) != 1){
            //-> модуль первого не меньше модуля второго
            result = result.TRANS_N_Z(m_.SUB_NN_N(other.m_));
            result.s_ = s_;
        } else {
            //-> модуль первого меньше модуля второго
            result = result.TRANS_N_Z((other.m_).SUB_NN_N(m_));
            result.s_ = other.s_;
        }
    }

    return result;
}

//Z-7
Integer Integer::SUB_ZZ_Z(const Integer& other) const{
    Integer result;

    // если вычитаемое и уменьшаемое разных знаков
    if (s_ != other.s_) {
        result = result.TRANS_N_Z(m_.ADD_NN_N(other.m_));
        result.s_ = s_;
    } else {
        // если уменьшаемое и вычитаемое одного знака и ->
        // знак берем от большего по модулю числа
        if (m_.COM_NN_D(other.m_) != 1){
            //-> модуль уменьшаемого не меньше модуля вычитаемого
            result = result.TRANS_N_Z(m_.SUB_NN_N(other.m_));
            result.s_ = s_;
        } else {
            //-> модуль уменьшаемого меньше модуля вычитаемого
            result = result.TRANS_N_Z((other.m_).SUB_NN_N(m_));
            result.s_ = other.s_;
        }
    }

    return result;
}

//Z-8
Integer Integer::MUL_ZZ_Z(const Integer& other) const{
    // перемножаем модули множителей
    Integer result = result.TRANS_N_Z(m_.MUL_NN_N(other.m_));
    // если знаки множителей различны,
    // то меняем знак результата на минус
    if (s_ != other.s_)
        result = result.MUL_ZM_Z();

    return result;
}

//Z-9
Integer Integer::DIV_ZZ_Z(const Integer& other) const {
    // если делитель равен 0
    if (!other.m_.NZER_N_B())
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");
    // делим модуль делимого на модуль делителя
    Integer div = div.TRANS_N_Z(m_.DIV_NN_N(other.m_));
    // находим остаток от деления модуля делимого
    // на модуль делителя
    Integer remain = remain.TRANS_N_Z(m_.MOD_NN_N(other.m_));
    // если знаки делимого и делителя различны
    if (s_ != other.s_){
        // если остаток ненулевой, то прибавляем к результату 1
        if (remain.m_.NZER_N_B())
            div = div.TRANS_N_Z(div.m_.ADD_1N_N());
        // меняем знак результата
        div = div.MUL_ZM_Z();
    }

    return div;
}

//Z-10
Integer Integer::MOD_ZZ_Z(const Integer& other) const {
    // если делитель равен 0
    if (!other.m_.NZER_N_B())
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");
    // находим остаток
    Integer remain = remain.TRANS_N_Z(m_.MOD_NN_N(other.m_));
    return remain;
}

std::ostream &operator<<(std::ostream &os, const Integer &number) {
    return os << number.as_string();
}