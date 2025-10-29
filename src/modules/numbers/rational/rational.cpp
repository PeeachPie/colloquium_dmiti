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

std::ostream &operator<<(std::ostream &os, const Rational &rational) {
    return os << rational.as_string();
}
