#include "polynomial.hpp"
#include <string>
#include <map>

Polynomial::Polynomial(): m_(0), c_({}) {}

Polynomial::Polynomial(int deg) {
    m_ = deg;
    c_.resize(m_ + 1, Rational());
} 

Polynomial::Polynomial(std::vector<std::string>& coeffs, std::vector<int> degs) {
    if (coeffs.size() != degs.size() || coeffs.empty())
        throw std::invalid_argument("Одному коэффициенту должна соответствовать одна степень!");
    std::map<int, Rational> cd;
    int max = -1;
    for (int i = 0; i < degs.size(); i++) {
        if (cd.find(degs[i]) == cd.end()) {
            cd[degs[i]] = Rational(coeffs[i]);
            max = std::max(max, degs[i]);
        } else
            throw std::invalid_argument("Одному коэффициенту должна соответствовать одна степень!");
    }
    m_ = max;
    c_.resize(m_ + 1);

    for (int i = 0; i <= m_; i++)
        c_[i] = cd[i];
}

std::string Polynomial::as_string() const {
    std::string result;
    for (int i = m_; i >= 0; i--) {
        if (c_[i].NZER_Q_B()) {
            if (c_[i].numerator().SGN_Z_D() == 1)
                result += "+ ";
            result += c_[i].as_string();
            if (i)
                result += " * x ^ " + std::to_string(i) + ' ';
        }
    }

    if (c_[m_].numerator().SGN_Z_D() == 1)
        result = result.substr(2, result.length());

    return result;
}
// P-1 Сложение многочленов
Polynomial Polynomial::ADD_PP_P(const Polynomial& other) const {
    Polynomial result;
    int max_degree = std::max(m_, other.m_);
    result.m_ = max_degree;
    // степень результата будет наибольшей из степеней двух полиномов
    result.c_.resize(max_degree + 1, Rational());

    for (int i = 0; i <= max_degree; ++i) {
        Rational a = (i <= m_) ? c_[i] : Rational();
        Rational b = (i <= other.m_) ? other.c_[i] : Rational();
        // для каждой степени i берем коэффициенты обоих полиномов,
        // если степени нет - считаем коэффициент 0
        result.c_[i] = a.ADD_QQ_Q(b);
    }

    return result;
}

// P-2 Вычитание многочленов
Polynomial Polynomial::SUB_PP_P(const Polynomial& other) const {
    Polynomial result;
    int max_degree = std::max(m_, other.m_);
    result.m_ = max_degree;
    // степень результата будет наибольшей из степеней двух полиномов
    result.c_.resize(max_degree + 1, Rational());


    for (int i = 0; i <= max_degree; ++i) {
        // проходимся по всем степеням 
        Rational a = (i <= m_) ? c_[i] : Rational();
        Rational b = (i <= other.m_) ? other.c_[i] : Rational();
        // для каждой степени i берем коэффициенты обоих полиномов,
        // если степени нет - считаем коэффициент 0
        result.c_[i] = a.SUB_QQ_Q(b);
    }

    return result;
}

// P-5 Старший коэффициент многочлена
Rational Polynomial::LED_P_Q() const {
    if (m_ < 0 || c_.empty()) {
        // проверяем, не нулевой ли полином
        return Rational(); 
    }
    return c_[m_];
}


// P-6 Степень многочлена
Natural Polynomial::DEG_P_N() const {
    for (int i = m_; i >= 0; --i) {
        // проходимся по массиву коэффициентов c_ с конца
        if (!(c_[i].INT_Q_B() && c_[i].numerator().SGN_Z_D() == 0)) {
            // если коэффициент не равен нулю, возвращаем его индекс
            return Natural(std::to_string(i));
        }
    }
    return Natural("0"); 
    // если ненулевых коэффициентов не найдено, 
    // то возвращаем нулевой полином
}



Polynomial Polynomial::NORM_P_P() const {
    Polynomial result = *this;

    // удаляем ведущие нули
    while (!result.c_.empty() &&
           !result.c_.back().NZER_Q_B()) {
        result.c_.pop_back();
    }

    // если полином пуст = нулевой
    if (result.c_.empty()) {
        result.m_ = 0;
        result.c_.assign(1, Rational(0));
        return result;
    }

    result.m_ = result.c_.size() - 1;

    return result;
}


std::ostream& operator << (std::ostream &os, const Polynomial& p) {
    return os << p.as_string();
}