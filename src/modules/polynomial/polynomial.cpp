#include "polynomial.hpp"

Polynomial::Polynomial(): m_(0), c_({{0, Rational()}}) {}

Polynomial::Polynomial(std::vector<std::pair<int, std::string>> polynomial) {
    for (auto &p: polynomial) {
        if (p.first < 0) {
            throw std::invalid_argument("Не допускаются отрицательные показатели степеней!");
        }

        c_[p.first] = c_[p.first].ADD_QQ_Q(Rational(p.second));
        m_ = std::max(m_, p.first);
    }

    Polynomial clean = this->NORM_P_P();

    m_ = clean.m_;
    c_ = clean.c_;
}

std::string Polynomial::as_string() const {
    Polynomial clean = this->NORM_P_P();

    std::string result;
    for (auto &p: clean.c_) {
        if (p.second.NZER_Q_B()) {
            if (p.second.numerator().SGN_Z_D() == 1)
                result += "+ ";
            result += p.second.as_string();
            if (p.first != 0)
                result += " * x ^ " + std::to_string(p.first) + ' ';
        }
    }

    if (c_.at(m_).numerator().SGN_Z_D() == 1)
        result = result.substr(2, result.length());
    if (result[result.length() - 1] == ' ') 
        result.pop_back();

    if (result == "") result = "0";

    return result;
}

bool Polynomial::NZER_P_B() const {
    return LED_P_Q().NZER_Q_B();
}

bool Polynomial::EQ_PP_B(const Polynomial& other) const {
    Polynomial p1 = this->NORM_P_P();
    Polynomial p2 = other.NORM_P_P();
    
    if (p1.m_ != p2.m_) {
        return false;
    }
    
    for (auto &p: p1.c_) {
        if (!p.second.EQ_QQ_B(p2.c_[p.first])) {
            return false;
        }
    }
    
    return true;
}

// P-1 Сложение многочленов
Polynomial Polynomial::ADD_PP_P(const Polynomial& other) const {
    Polynomial result = *this;

    for (auto &p: other.c_) {
        // если такой степени еще нет - считаем ее 0
        Rational a = (result.c_.find(p.first) != result.c_.end()) ? result.c_.at(p.first) : Rational();

        // прибавляем к ответу
        result.c_[p.first] = a.ADD_QQ_Q(p.second);
    }

    result = result.NORM_P_P();

    return result;
}

// P-2 Вычитание многочленов
Polynomial Polynomial::SUB_PP_P(const Polynomial& other) const {
    Polynomial result = *this;

    for (auto &p: other.c_) {
        // если такой степени еще нет - считаем ее 0
        Rational a = (result.c_.find(p.first) != result.c_.end()) ? result.c_.at(p.first) : Rational();

        // прибавляем к ответу
        result.c_[p.first] = a.SUB_QQ_Q(p.second);
    }

    result = result.NORM_P_P();

    return result;
}

// P-3
Polynomial Polynomial::MUL_PQ_P(const Rational& rational) const {
    // если домножаем на ноль - возвращаем сразу нулевой полином
    if (!rational.NZER_Q_B()) {
        return Polynomial();
    }

    Polynomial result = *this;
    // если полином ненулевой
    if (result.NZER_P_B()) {
        // умножаем каждый коэффициент исходного полинома
        // на заданное число
        for (auto&p : result.c_)
            p.second = p.second.MUL_QQ_Q(rational);
    }

    return result;
}

// P-4
Polynomial Polynomial::MUL_Pxk_P(int k) const {
    // если k меньше 0, то ошибка
    if (k < 0)
        throw std::invalid_argument("Нельзя домножить на отрицательную степень");
    // если степень множителя 0 или полином нулевой,
    // то ничего не делаем
    if (k == 0 || !NZER_P_B())
        return *this;

    Polynomial result;
    // увеличиваем степени на k
    for (auto &p: c_)
        result.c_[p.first + k] = p.second;

    return result;
}

// P-5 Старший коэффициент многочлена
Rational Polynomial::LED_P_Q() const {
    // нормируем полином
    Polynomial clean = this->NORM_P_P();
    
    return clean.c_[clean.m_];
}

// P-6 Степень многочлена
Natural Polynomial::DEG_P_N() const {
    Polynomial clean = this->NORM_P_P();

    return Natural(std::to_string(clean.m_));
}

// P-7
std::pair<Rational, Polynomial> Polynomial::FAC_P_Q() const {
    Polynomial clean = this->NORM_P_P();
    // если полином нулевой, то ошибка
    if (!NZER_P_B())
        throw std::invalid_argument("Полином нулевой!");

    // начальные значения НОД и НОК
    Natural GCF = Natural("0");
    Natural LCM = Natural("1");

    // проходимся по всем коэффициентам
    for (auto &p : clean.c_) {
        // получаем числитель и знаменатель
        Natural current_numerator = p.second.numerator().ABS_Z_N();
        Natural current_denominator = p.second.denominator();

        GCF = GCF.GCF_NN_N(p.second.numerator().ABS_Z_N());
        LCM = LCM.LCM_NN_N(p.second.denominator());
    }

    // преобразуем полученные НОД и НОК в рациональное число
    Rational GL = Rational(Integer(GCF.as_string()), LCM);

    // делим каждый коэффициент полинома на полученную дробь
    for (auto &p : clean.c_)
        p.second = p.second.DIV_QQ_Q(GL);

    return std::make_pair(GL, clean);
}

// P-8 | Умножение двух многочленов
Polynomial Polynomial::MUL_PP_P(const Polynomial& other) const {
    Polynomial a = this->NORM_P_P();
    Polynomial b = other.NORM_P_P();

    if (!a.NZER_P_B() || !b.NZER_P_B())
        return Polynomial();

    Polynomial result;

    for (auto &p: a.c_) {
        for (auto &q: b.c_) {
            int k = p.first + q.first;
            Rational prod = p.second.MUL_QQ_Q(q.second);
            result.c_[k] = result.c_[k].ADD_QQ_Q(prod);
        }
    }

    result.m_ = a.m_ + b.m_;

    return result;
}

// P-9 | Частное от деления многочлена на многочлен (с остатком)
Polynomial Polynomial::DIV_PP_P(const Polynomial& other) const {
    if (!other.NZER_P_B())
        throw std::invalid_argument("Деление на нулевой многочлен недопустимо!");

    Polynomial dividend = this->NORM_P_P();
    Polynomial divisor  = other.NORM_P_P();

    if (dividend.m_ < divisor.m_)
        return Polynomial();

    Polynomial quotient;

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
    Polynomial product = other.MUL_PP_P(quotient);
    Polynomial remainder = this->SUB_PP_P(product);
    
    return remainder.NORM_P_P();
}

// P-11 | НОД многочленов (алгоритм Евклида)
Polynomial Polynomial::GCF_PP_P(const Polynomial& other) const {
    Polynomial a = this->NORM_P_P();
    Polynomial b = other.NORM_P_P();
    
    // алгоритм Евклида: НОД(a, b) = НОД(b, a mod b)
    while (b.NZER_P_B()) { // пока остаток не станет нулевым
        const Polynomial remainder = a.MOD_PP_P(b);
        a = b;
        b = remainder;
    }
    
    Rational sgn = Rational::TRANS_Z_Q(
        Integer(std::to_string(a.LED_P_Q().numerator().SGN_Z_D()))
    );
    // последний ненулевой остаток = НОД
    return a.FAC_P_Q().second.MUL_PQ_P(sgn);
}

// P-12
Polynomial Polynomial::DER_P_P() const {
    Polynomial clean = this->NORM_P_P();

    Polynomial result;
    for (auto &p: clean.c_) {
        // если константа - не учитываем
        if (p.first == 0) continue;

        // переводим степень в рациоанальное число
        Rational d = Rational::TRANS_Z_Q(Integer(std::to_string(p.first)));

        result.c_[p.first - 1] = p.second.MUL_QQ_Q(d);
    }

    result.m_ = clean.m_ - 1;

    return result;
}

// P-13 | Преобразование многочлена — кратные корни в простые
// Алгоритм: P_new = P / НОД(P, P')
Polynomial Polynomial::NMR_P_P() const {
    if (!NZER_P_B() || m_ == 0) {
        return *this;
    }

    const Polynomial derivative = this->DER_P_P();

    Polynomial gcd = this->GCF_PP_P(derivative);
    
    // делим исходный полином на НОД (убираем кратные корни)
    const Polynomial result = this->DIV_PP_P(gcd);
    
    return result.NORM_P_P();
}

Polynomial Polynomial::NORM_P_P() const {
    Polynomial clean;
    // оставляем только члены с ненулевыми коэфициентами
    for (auto &p: c_) {
        if (p.second.NZER_Q_B()) {
            clean.c_[p.first] = p.second;
        }
    }

    // если полином пуст = нулевой
    if (clean.c_.empty()) {
        clean.c_[0] = Rational();
    }

    // степень - коэфициент первого члена
    clean.m_ = clean.c_.begin()->first;

    return clean;
}


std::ostream& operator << (std::ostream &os, const Polynomial& p) {
    return os << p.as_string();
}

bool operator==(const Polynomial& p1, const Polynomial& p2) {
    return p1.EQ_PP_B(p2);
}