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

    this->NORM_P_P();
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

// P-3
Polynomial Polynomial::MUL_PQ_P(const Rational& rational) const {
    Polynomial result = *this;
    // умножаем каждый коэффициент исходного полинома
    // на заданное число
    for (auto& coeff : result.c_)
        coeff = coeff.MUL_QQ_Q(rational);
    result = result.NORM_P_P();
    return result;
}

// P-4
Polynomial Polynomial::MUL_Pxk_P(int k) const {
    // если k меньше 0, то ошибка
    if (k < 0)
        throw std::invalid_argument("Степень должна быть натуральной или 0!");
    // если степень множителя 0, то ничего не делаем
    if (k == 0)
        return *this;
    // создаем нулевой полином нового размера
    Polynomial result(m_ + k);
    // переносим коэффициенты на k вправо
    for (int i = 0; i <= m_; i++)
        result.c_[i + k] = c_[i];
    result = result.NORM_P_P();
    return result;
}

// P-7
std::pair<Rational, Polynomial> Polynomial::FAC_P_Q() const {
    if (!NZER_P_B())
        throw std::invalid_argument("Полином нулевой!");

    std::vector<Natural> numerators;
    std::vector<Natural> denominators;
    // проходимся по всем коэффициентам
    for (Rational coeff : c_) {
        // если коэффициент ненулевой,
        if (coeff.NZER_Q_B()) {
            // то его получаем числитель и знаменатель
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
            if (cn_found != numerators.end())
                numerators.push_back(current_numerator);
            if (cd_found != denominators.end())
                denominators.push_back(current_denominator);
        }
    }
    // количество полученных уникальных ненулевых коэффициентов
    int size = numerators.size();

    // ищем НОД и НОК
    Natural GCF = numerators[0];
    Natural LCM = denominators[0];
    // если уникальных ненулевых коэффициентов больше одного
    if (size > 1) {
        for (int i = 1; i < size; i++) {
            // ищем НОД текущего НОД и нового числа
            GCF = GCF.GCF_NN_N(numerators[i]);
            // ищем НОК текущего НОК и нового числа
            LCM = LCM.LCM_NN_N(denominators[i]);
        }
    }

    // преобразуем полученные НОД и НОК в рациональное число
    Rational GL = Rational(Integer(GCF.as_string()), LCM);

    // делим каждый коэффициент полинома на полученную дробь
    Polynomial result = *this;
    for (auto& coeff : result.c_)
        coeff = coeff.DIV_QQ_Q(GL);

    return std::make_pair(GL, result);
}

// P-12
Polynomial Polynomial::DER_P_P() const {
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