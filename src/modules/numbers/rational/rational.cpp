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

std::ostream &operator<<(std::ostream &os, const Rational &rational) {
    return os << rational.as_string();
}
