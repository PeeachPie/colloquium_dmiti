#include "integer.hpp"

Integer::Integer() : m_(), s_(false) {}

Integer::Integer(std::string number):
    s_(number[0] == '-'),
    m_(number[0] == '-' ? number.substr(1) : number) 
{}

Integer::Integer(bool s, std::string number):
    s_(s),
    m_(number)
{}

std::string Integer::as_string() const {
    // сохраняем поле Natural m_ как строку
    std::string str = m_.as_string();

    // если данное целое число отрицательное,
    // то вставляем в начало минус
    if (s_)
        str = '-' + str;

    return str;
}

//Z-0
int Integer::COM_ZZ_D(const Integer &other) const {
    if (s_ == other.s_) {
        if (m_.COM_NN_D(other.m_) == 2) {
            if (s_)
                return 1;
            else
                return 2;
        } else if (m_.COM_NN_D(other.m_) == 1) {
            if (s_)
                return 2;
            else
                return 1;
        } else
            return 0;
    } else {
        if (s_)
            return 1;
        else
            return 2;
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
    Integer result = *this;
    result.s_ = !result.s_; //просто меняем знак
    return result;
}

//Z-4
Integer Integer::TRANS_N_Z(const Natural& natural){ 
    return Integer(natural.as_string()); // создаем целое число из заданного натурального
}

//Z-5
Natural Integer::TRANS_Z_N() const{
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
    // если знаки множителей различны, то меняем
    // знак результата на минус
    if (s_ != other.s_)
        result = result.MUL_ZM_Z();

    return result;
}

//Z-9
Integer Integer::DIV_ZZ_Z(const Integer& other) const {
    // делим модуль делимого на модуль делителя
    Integer result = result.TRANS_N_Z(m_.DIV_NN_N(other.m_));
    // если знаки делимого и делителя различны, то
    // добавляем к результату 1 и меняем его знак на минус
    if (s_ != other.s_) {
        result.m_ = result.m_.ADD_1N_N();
        result = result.MUL_ZM_Z();
    }

    return result;
}

//Z-10
Integer Integer::MOD_ZZ_Z(const Integer& other) const {
    // если знаки делимого и делителя одинаковы, то
    // остаток ищем как для натуральных
    if (s_ == other.s_) {
        Integer result = result.TRANS_N_Z(m_.MOD_NN_N(other.m_));
        return result;
    }

    // иначе
    // находим отрицательное неполное частное
    Integer div = DIV_ZZ_Z(other);

    // находим произведение неполного отрицательного частного
    // и делителя
    Integer closest_max = div.MUL_ZZ_Z(other);
    // если результат отрицательный - меняем знак
    if (closest_max.s_)
        closest_max = closest_max.MUL_ZM_Z();

    // сохраняем делимое в отдельную переменную
    Integer divisible = *this;
    // если результат отрицательный - меняем знак
    if (divisible.s_)
        divisible = divisible.MUL_ZM_Z();

    // находим разность большего и меньшего,
    // сохраняя положительный остаток
    Integer result = closest_max.SUB_ZZ_Z(divisible);

    return result;
}

std::ostream &operator<<(std::ostream &os, const Integer &number) {
    return os << number.as_string();
}