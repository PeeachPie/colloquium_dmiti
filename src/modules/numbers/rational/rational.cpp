#include "rational.hpp"
#include <sstream>
#include <stdexcept>

// Конструктор по умолчанию: создаёт 0/1
Rational::Rational() : p_(Integer("0")), q_(Natural("1")) {
}

// Конструктор из числителя и знаменателя
Rational::Rational(const Integer &numerator, const Natural &denominator)
    : p_(numerator), q_(denominator) {
    if (!q_.NZER_N_B()) {
        throw std::invalid_argument("Знаменатель не может быть равен нулю!");
    }
}

// Конструктор из строки вида "p/q" или "p"
Rational::Rational(const std::string &str) {
    if (const size_t slash_pos = str.find('/'); slash_pos == std::string::npos) {
        p_ = Integer(str);
        q_ = Natural("1");
    } else {
        const std::string numerator_str = str.substr(0, slash_pos);
        const std::string denominator_str = str.substr(slash_pos + 1);

        p_ = Integer(numerator_str);
        q_ = Natural(denominator_str);

        if (!q_.NZER_N_B()) {
            throw std::invalid_argument("Знаменатель не может быть равен нулю!");
        }
    }
}

std::string Rational::as_string() const {
    std::string result = p_.as_string();

    if (q_.as_string() != "1") {
        result += "/" + q_.as_string();
    }

    return result;
}

bool Rational::NZER_Q_B() const {
    return p_.ABS_Z_N().NZER_N_B();
}

bool Rational::EQ_QQ_B(const Rational& other) const {
    const Rational diff = this->SUB_QQ_Q(other);
    return !diff.NZER_Q_B();
}

// Q-1 | Сокращение дроби
Rational &Rational::RED_Q_Q() {
    const Natural abs_numerator = p_.ABS_Z_N();
    const Natural gcd = abs_numerator.GCF_NN_N(q_);

    if (gcd.as_string() == "1") {
        return *this;
    }

    const Natural new_abs_numerator = abs_numerator.DIV_NN_N(gcd);
    const Natural new_denominator = q_.DIV_NN_N(gcd);

    const bool is_negative = (p_.SGN_Z_D() == -1);

    Integer new_numerator(new_abs_numerator.as_string());
    if (is_negative) {
        new_numerator = new_numerator.MUL_ZM_Z();
    }

    p_ = new_numerator;
    q_ = new_denominator;

    return *this;
}

// Q-2 | Проверка сокращенного дробного на целое
bool Rational::INT_Q_B() const {
    return q_.as_string() == "1";
}

// Q-3 | Преобразование целого в дробное (статический метод)
Rational Rational::TRANS_Z_Q(const Integer &integer) {
    return Rational(integer, Natural("1"));
}

// Q-4 | Преобразование сокращенного дробного в целое (если знаменатель равен 1)
Integer Rational::TRANS_Q_Z() const {
    if (!INT_Q_B()) {
        throw std::invalid_argument("Рациональное число не является целым!");
    }
    return p_;
}

// Q-5 | Сложение дробей
Rational Rational::ADD_QQ_Q(const Rational &other) const {
    Natural lcm_denominator = q_.LCM_NN_N(other.q_);

    Natural factor1 = lcm_denominator.DIV_NN_N(q_);
    Natural factor2 = lcm_denominator.DIV_NN_N(other.q_);

    auto int_factor1 = Integer(factor1.as_string());
    auto int_factor2 = Integer(factor2.as_string());

    Integer new_numerator1 = p_.MUL_ZZ_Z(int_factor1);
    Integer new_numerator2 = other.p_.MUL_ZZ_Z(int_factor2);

    Integer result_numerator = new_numerator1.ADD_ZZ_Z(new_numerator2);

    Rational result(result_numerator, lcm_denominator);

    result.RED_Q_Q();

    return result;
}

// Q-6 | Вычитание дробей
Rational Rational::SUB_QQ_Q(const Rational &other) const {
    const Rational negated_other(other.p_.MUL_ZM_Z(), other.q_);
    return ADD_QQ_Q(negated_other);
}

// Q-7 | Умножение дробей
// Алгоритм с предварительным сокращением:
// (a/b) * (c/d) = (a/gcd(a,d)) * (c/gcd(c,b)) / ((b/gcd(c,b)) * (d/gcd(a,d)))
Rational Rational::MUL_QQ_Q(const Rational &other) const {
    Natural abs_p = p_.ABS_Z_N();
    Natural abs_other_p = other.p_.ABS_Z_N();
    
    Natural gcd1 = abs_p.GCF_NN_N(other.q_);      // gcd(|a|, d)
    Natural gcd2 = abs_other_p.GCF_NN_N(q_);     // gcd(|c|, b)
    
    // Сокращаем числители и знаменатели на НОД
    Natural reduced_abs_p = abs_p.DIV_NN_N(gcd1);        // |a| / gcd1
    Natural reduced_abs_other_p = abs_other_p.DIV_NN_N(gcd2);  // |c| / gcd2
    Natural reduced_q = q_.DIV_NN_N(gcd2);               // b / gcd2
    Natural reduced_other_q = other.q_.DIV_NN_N(gcd1);   // d / gcd1
    
    Integer temp;
    Integer reduced_p = temp.TRANS_N_Z(reduced_abs_p);
    Integer reduced_other_p = temp.TRANS_N_Z(reduced_abs_other_p);
    
    if (p_.SGN_Z_D() == -1) {
        reduced_p = reduced_p.MUL_ZM_Z();
    }
    if (other.p_.SGN_Z_D() == -1) {
        reduced_other_p = reduced_other_p.MUL_ZM_Z();
    }
    
    Integer result_numerator = reduced_p.MUL_ZZ_Z(reduced_other_p);
    Natural result_denominator = reduced_q.MUL_NN_N(reduced_other_q);
    
    Rational result(result_numerator, result_denominator);
    
    result.RED_Q_Q();
    
    return result;
}

// Q-8 | Деление дробей
Rational Rational::DIV_QQ_Q(const Rational &other) const {
    if (other.p_.SGN_Z_D() == 0) {
        throw std::invalid_argument("Деление на ноль!");
    }

    // c/d -> d/c (с учётом знака)
    const Natural abs_other_numerator = other.p_.ABS_Z_N();
    Integer temp;
    Integer reciprocal_numerator = temp.TRANS_N_Z(other.q_);
    if (other.p_.SGN_Z_D() == -1) {
        reciprocal_numerator = reciprocal_numerator.MUL_ZM_Z();
    }
    const Rational reciprocal(reciprocal_numerator, abs_other_numerator);
    
    return MUL_QQ_Q(reciprocal);
}

std::ostream &operator<<(std::ostream &os, const Rational &rational) {
    return os << rational.as_string();
}

bool operator==(const Rational &r1, const Rational &r2) {
    return r1.EQ_QQ_B(r2);
}
