#include "polynomial.hpp"
#include <iostream>

Polynomial::Polynomial(): m_(0) {}

Polynomial::Polynomial(std::vector<std::pair<Rational, int>> p) {
    m_ = p[p.size() - 1].second;
    c_.resize(m_);
    for (std::pair cd : p)
        c_[cd.second] = cd.first;
}

// P-3
Polynomial Polynomial::MUL_PQ_P(const Rational& rational) const {
    Polynomial result = *this;
    // умножаем каждый коэффициент полинома
    // на заданное число
    for (Rational coeff : result.c_)
        coeff = coeff.MUL_QQ_Q(rational);
    return result;
}

// P-4
Polynomial Polynomial::MUL_Pxk_P(int k) const {
    // если степень множителя 0, то ничего не делаем
    if (k == 0)
        return *this;
    Polynomial result;
    // степень результата равна сумме степени множителя
    // и старшей степени полинома
    result.m_ = m_ + k;
    // подгоняем размер массива коэффициентов под новую степень
    result.c_.resize(result.m_);
    // переносим коэффициенты на k вправо
    for (int i = 0; i <= m_; i++)
        result.c_[i + k] = c_[i];
    return result;
}

// P-7
std::pair<Rational, Polynomial> Polynomial::FAC_P_Q() const {
    // если в полиноме только один коэффициент, то ничего не делаем
    if (m_ == 0)
        return std::make_pair(c_[0], *this);
    std::vector<Natural> numerators;
    std::vector<Natural> denominators;
    // проходимся по всем коэффициентам
    for (Rational coeff : c_) {
        // получаем числитель и знаменатель каждого коэффициента
        Natural current_numerator = coeff.numerator();
        Natural current_denominator = coeff.denominator();
        // если коэффициент не равен 0
        if (current_numerator.NZER_N_B()){
            // ищем числитель и знаменатель в векторах
            int cn_found = std::count(numerators.begin(), numerators.end(), current_numerator);
            int cd_found = std::count(denominators.begin(), denominators.end(), current_denominator);
            // если таких чисел еще не было - добавляем
            if (!cn_found)
                numerators.push_back(current_numerator);
            if (!cd_found)
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
    for (Rational coeff : result.c_)
        coeff = coeff.DIV_QQ_Q(GL);

    return std::make_pair(GL, result);
}

// P-12
Polynomial Polynomial::DER_P_P() const {
    Polynomial result;
    // степень производной будет на 1 меньше
    // степени исходного полинома
    result.m_ = m_ - 1;
    // подгоняем размер массива коэффициентов под новую степень
    result.c_.resize(result.m_);
    for (int i = 0; i <= result.m_; i++){
        // перемещаем коэффициенты исходного
        // полинома на 1 влево
        result.c_[i] = c_[i + 1];
        Integer numerator = result.c_[i].numerator();
        // если текущий коэффициент не равен 0
        if (numerator.ABS_Z_N().NZER_N_B()) {
            // преобразуем бывшую степень в рациональное число
            Rational d = d.TRANS_Z_Q(Integer(std::to_string(i + 1)));
            // домножаем коэффициент на бывшую степень
            result.c_[i] = result.c_[i].MUL_QQ_Q(d);
        }
    }
    return result;
}

void Polynomial::print_polynomial() {
    for (int i = m_; i >= 0; i--) {
        if (c_[i].numerator().ABS_Z_N().NZER_N_B())
            std::cout << c_[i] << " * x ^ " << i;
        if (i)
            std::cout << " + ";
    }
}