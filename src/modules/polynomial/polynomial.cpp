#include "polynomial.hpp"
#include <iostream>
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
    if (result[result.length() - 1] == ' ') 
        result.pop_back();

    return result;
}

bool Polynomial::NZER_P_B() const {
    // проходимся по всем коэффициентам
    for (Rational coeff : c_) {
        // если нашли хотя бы один ненулевой,
        // то полином ненулевой
        if (coeff.NZER_Q_B())
            return true;
    }
    // иначе нулевой
    return false;
}

bool Polynomial::EQ_PP_B(const Polynomial& other) const {
    Polynomial p1 = this->NORM_P_P();
    Polynomial p2 = other.NORM_P_P();
    
    if (p1.m_ != p2.m_) {
        return false;
    }
    
    for (int i = 0; i <= p1.m_; ++i) {
        if (!p1.c_[i].EQ_QQ_B(p2.c_[i])) {
            return false;
        }
    }
    
    return true;
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

// P-3
Polynomial Polynomial::MUL_PQ_P(const Rational& rational) const {
    Polynomial result = *this;
    // если полином ненулевой
    if (result.NZER_P_B()) {
        // умножаем каждый коэффициент исходного полинома
        // на заданное число
        for (auto& coeff : result.c_)
            coeff = coeff.MUL_QQ_Q(rational);
    }
    return result;
}

// P-4
Polynomial Polynomial::MUL_Pxk_P(int k) const {
    // если k меньше 0, то ошибка
    if (k < 0)
        throw std::invalid_argument("Степень должна быть натуральной или 0!");
    // если степень множителя 0 или полином нулевой,
    // то ничего не делаем
    if (k == 0 || !NZER_P_B())
        return *this;
    // создаем нулевой полином нового размера
    Polynomial result(m_ + k);
    // переносим коэффициенты на k вправо
    for (int i = 0; i <= m_; i++)
        result.c_[i + k] = c_[i];
    return result;
}

// P-5 Старший коэффициент многочлена
Rational Polynomial::LED_P_Q() const {
    if (m_ < 0 || c_.empty()) {
        // проверяем, не нулевой ли полином
        return Rational();
    }

    for (int i = m_; i >= 0; --i) {
        if (c_[i].NZER_Q_B()) {
            return c_[i];
        }
    }
    
    return Rational();
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

// P-7
std::pair<Rational, Polynomial> Polynomial::FAC_P_Q() const {
    // если полином нулевой, то ошибка
    if (!NZER_P_B())
        throw std::invalid_argument("Полином нулевой!");

    std::vector<Natural> numerators;
    std::vector<Natural> denominators;
    // проходимся по всем коэффициентам
    for (Rational coeff : c_) {
        // если коэффициент ненулевой,
        if (coeff.NZER_Q_B()) {
            // то получаем его числитель и знаменатель
            Natural current_numerator = coeff.numerator().ABS_Z_N();
            Natural current_denominator = coeff.denominator();

            // если массивы пустые - просто добавляем значения
            if (!numerators.size() && !denominators.size()) {
                numerators.push_back(current_numerator);
                denominators.push_back(current_denominator);
                continue;
            }

            // иначе ищем числитель и знаменатель в массивах
            auto cn_found{ std::find(numerators.begin(), numerators.end(), current_numerator) };
            auto cd_found{ std::find(denominators.begin(), denominators.end(), current_denominator) };
            // если таких чисел еще не было - добавляем
            if (cn_found == numerators.end())
                numerators.push_back(current_numerator);
            if (cd_found == denominators.end())
                denominators.push_back(current_denominator);
        }
    }
    // ищем НОД и НОК
    Natural GCF = numerators[0];
    Natural LCM = denominators[0];
    // количество уникальных числителей
    int n_size = numerators.size();
    // количество уникальных знаменателей
    int d_size = denominators.size();
    // если уникальных числителей больше одного
    if (n_size > 1) {
        // ищем их НОД
        for (int i = 1; i < n_size; i++)
            GCF = GCF.GCF_NN_N(numerators[i]);
    }
    // если уникальных знаменателей больше одного
    if (d_size > 1) {
        // ищем их НОК
        for (int i = 1; i < d_size; i++)
            LCM = LCM.LCM_NN_N(denominators[i]);
    }

    // преобразуем полученные НОД и НОК в рациональное число
    Rational GL = Rational(Integer(GCF.as_string()), LCM);

    // делим каждый коэффициент полинома на полученную дробь
    Polynomial result = *this;
    for (auto& coeff : result.c_)
        coeff = coeff.DIV_QQ_Q(GL);

    return std::make_pair(GL, result);
}

// P-8 | Умножение двух многочленов
Polynomial Polynomial::MUL_PP_P(const Polynomial& other) const {
    if (!NZER_P_B() || !other.NZER_P_B())
        return Polynomial();

    int new_deg = m_ + other.m_;
    Polynomial result(new_deg);

    for (int i = 0; i <= m_; i++) {
        for (int j = 0; j <= other.m_; j++) {
            Rational prod = c_[i].MUL_QQ_Q(other.c_[j]);
            result.c_[i + j] = result.c_[i + j].ADD_QQ_Q(prod);
        }
    }

    return result.NORM_P_P();
}

// P-9 | Частное от деления многочлена на многочлен (с остатком)
Polynomial Polynomial::DIV_PP_P(const Polynomial& other) const {
    if (!other.NZER_P_B())
        throw std::invalid_argument("Деление на нулевой многочлен недопустимо!");

    Polynomial dividend = this->NORM_P_P();
    Polynomial divisor  = other.NORM_P_P();

    if (dividend.m_ < divisor.m_)
        return Polynomial();

    Polynomial quotient(dividend.m_ - divisor.m_);

    while (dividend.m_ >= divisor.m_ && dividend.NZER_P_B()) {
        int deg_diff = dividend.m_ - divisor.m_;
        Rational factor = dividend.c_[dividend.m_].DIV_QQ_Q(divisor.c_[divisor.m_]);

        quotient.c_[deg_diff] = quotient.c_[deg_diff].ADD_QQ_Q(factor);

        Polynomial temp = divisor.MUL_PQ_P(factor).MUL_Pxk_P(deg_diff);
        dividend = dividend.SUB_PP_P(temp).NORM_P_P();
    }

    return quotient.NORM_P_P();
}

// P-10 | Остаток от деления многочлена на многочлен
Polynomial Polynomial::MOD_PP_P(const Polynomial& other) const {
    if (!other.NZER_P_B())
        throw std::invalid_argument("Деление на нулевой многочлен недопустимо!");

    Polynomial quotient = this->DIV_PP_P(other);
    Polynomial remainder = this->SUB_PP_P(other.MUL_PP_P(quotient));
    return remainder.NORM_P_P();
}

// P-11 | НОД многочленов (алгоритм Евклида)
Polynomial Polynomial::GCF_PP_P(const Polynomial& other) const {
    Polynomial a = this->NORM_P_P();
    Polynomial b = other.NORM_P_P();

    if (!a.NZER_P_B() && !b.NZER_P_B()) {
        return Polynomial();
    }
    if (!a.NZER_P_B()) {
        return b;
    }
    if (!b.NZER_P_B()) {
        return a;
    }
    
    // алгоритм Евклида: НОД(a, b) = НОД(b, a mod b)
    while (b.NZER_P_B()) { // пока остаток не станет нулевым
        const Polynomial remainder = a.MOD_PP_P(b);
        a = b;
        b = remainder;
    }
    
    // последний ненулевой остаток = НОД
    return a;
}

// P-12
Polynomial Polynomial::DER_P_P() const {
    // если полином нулевой - ничего не делаем
    if (!NZER_P_B())
        return *this;
    // создаем нулевой полином степени на 1 меньше
    Polynomial result(m_ - 1);
    for (int i = 0; i <= result.m_; i++){
        // перемещаем коэффициенты исходного
        // полинома на 1 влево
        result.c_[i] = c_[i + 1];
        // если текущий коэффициент не равен 0
        if (result.c_[i].NZER_Q_B()) {
            // преобразуем бывшую степень в рациональное число
            Rational d = d.TRANS_Z_Q(Integer(std::to_string(i + 1)));
            // домножаем коэффициент на бывшую степень
            result.c_[i] = result.c_[i].MUL_QQ_Q(d);
        }
    }
    return result;
}

// P-13 | Преобразование многочлена — кратные корни в простые
// Алгоритм: P_new = P / НОД(P, P')
Polynomial Polynomial::NMR_P_P() const {
    if (!NZER_P_B() || m_ == 0) {
        return *this;
    }

    const Polynomial derivative = this->DER_P_P();
    if (!derivative.NZER_P_B()) {
        return *this;
    }

    Polynomial gcd = this->GCF_PP_P(derivative);
    if (!gcd.NZER_P_B()) {
        return *this;
    }
    
    // делим исходный полином на НОД (убираем кратные корни)
    const Polynomial result = this->DIV_PP_P(gcd);
    
    return result.NORM_P_P();
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

bool operator==(const Polynomial& p1, const Polynomial& p2) {
    return p1.EQ_PP_B(p2);
}