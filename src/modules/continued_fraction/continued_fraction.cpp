#include "continued_fraction.hpp"
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <set>


ContinuedFraction::ContinuedFraction() 
    : coefficients_({Integer("0")}), period_start_(-1) {}

ContinuedFraction::ContinuedFraction(const std::vector<Integer>& coefficients)
    : coefficients_(coefficients), period_start_(-1) {
    if (coefficients_.empty()) {
        coefficients_.push_back(Integer("0"));
    }
}

ContinuedFraction::ContinuedFraction(const std::vector<int>& coefficients)
    : period_start_(-1) {
    for (int c : coefficients) {
        coefficients_.push_back(Integer(std::to_string(c)));
    }
    if (coefficients_.empty()) {
        coefficients_.push_back(Integer("0"));
    }
}

ContinuedFraction::ContinuedFraction(const std::vector<Integer>& coefficients, int period_start)
    : coefficients_(coefficients), period_start_(period_start) {
    if (coefficients_.empty()) {
        coefficients_.push_back(Integer("0"));
        period_start_ = -1;
    }
}

// CF-1 | Преобразование рационального числа в цепную дробь
// Алгоритм Евклида для получения частичных частных
ContinuedFraction ContinuedFraction::FROM_Q_CF(const Rational& rational) {
    std::vector<Integer> coeffs;
    
    Integer p = rational.numerator();
    Natural q = rational.denominator();
    
    // Алгоритм: a_i = floor(p/q), затем p, q = q, p mod q
    while (q.NZER_N_B()) {
        // целая часть p/q
        Integer quotient = p.DIV_ZZ_Z(Integer(q.as_string()));
        coeffs.push_back(quotient);
        
        // остаток
        Integer remainder = p.MOD_ZZ_Z(Integer(q.as_string()));
        
        // p = q, q = остаток
        p = Integer(q.as_string());
        q = Natural(remainder.ABS_Z_N().as_string());
    }
    
    return ContinuedFraction(coeffs);
}

// CF-2 | Преобразование цепной дроби в рациональное число
// Используем рекуррентное соотношение: h_n/k_n
Rational ContinuedFraction::TO_CF_Q() const {
    if (coefficients_.empty()) {
        return Rational();
    }
    
    // вычисляем снизу вверх, начинаем с последнего коэффициента
    Integer h_prev = Integer("1");  // h_{-1} = 1
    Integer k_prev = Integer("0");  // k_{-1} = 0
    Integer h_curr = coefficients_[0];  // h_0 = a_0
    Integer k_curr = Integer("1");  // k_0 = 1
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        // h_n = a_n * h_{n-1} + h_{n-2}
        // k_n = a_n * k_{n-1} + k_{n-2}
        Integer h_new = coefficients_[i].MUL_ZZ_Z(h_curr).ADD_ZZ_Z(h_prev);
        Integer k_new = coefficients_[i].MUL_ZZ_Z(k_curr).ADD_ZZ_Z(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
    }
    
    // результат: h_n / k_n
    if (k_curr.SGN_Z_D() == 0) {
        throw std::runtime_error("Знаменатель равен нулю");
    }
    
    if (k_curr.SGN_Z_D() < 0) {
        h_curr = h_curr.MUL_ZM_Z();
        k_curr = k_curr.MUL_ZM_Z();
    }
    
    Rational result(h_curr, k_curr.ABS_Z_N());
    result.RED_Q_Q();
    return result;
}


// CF-3 | Получение последовательности конвергентов
std::vector<Rational> ContinuedFraction::CONVERGENTS_CF() const {
    std::vector<Rational> convergents;
    
    if (coefficients_.empty()) {
        return convergents;
    }
    
    Integer h_prev = Integer("1");  // h_{-1} = 1
    Integer k_prev = Integer("0");  // k_{-1} = 0
    Integer h_curr = coefficients_[0];  // h_0 = a_0
    Integer k_curr = Integer("1");  // k_0 = 1
    
    if (k_curr.SGN_Z_D() != 0) {
        Integer h = h_curr, k = k_curr;
        if (k.SGN_Z_D() < 0) {
            h = h.MUL_ZM_Z();
            k = k.MUL_ZM_Z();
        }
        Rational conv(h, k.ABS_Z_N());
        conv.RED_Q_Q();
        convergents.push_back(conv);
    }
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        Integer h_new = coefficients_[i].MUL_ZZ_Z(h_curr).ADD_ZZ_Z(h_prev);
        Integer k_new = coefficients_[i].MUL_ZZ_Z(k_curr).ADD_ZZ_Z(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
        
        if (k_curr.SGN_Z_D() != 0) {
            Integer h = h_curr, k = k_curr;
            if (k.SGN_Z_D() < 0) {
                h = h.MUL_ZM_Z();
                k = k.MUL_ZM_Z();
            }
            Rational conv(h, k.ABS_Z_N());
            conv.RED_Q_Q();
            convergents.push_back(conv);
        }
    }
    
    return convergents;
}

// CF-4 | Получение n-го конвергента
Rational ContinuedFraction::CONVERGENT_N_CF(int n) const {
    if (n < 0 || n >= static_cast<int>(coefficients_.size())) {
        throw std::out_of_range("Индекс конвергента вне диапазона");
    }
    
    Integer h_prev = Integer("1");
    Integer k_prev = Integer("0");
    Integer h_curr = coefficients_[0];
    Integer k_curr = Integer("1");
    
    for (int i = 1; i <= n; ++i) {
        Integer h_new = coefficients_[i].MUL_ZZ_Z(h_curr).ADD_ZZ_Z(h_prev);
        Integer k_new = coefficients_[i].MUL_ZZ_Z(k_curr).ADD_ZZ_Z(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
    }
    
    if (k_curr.SGN_Z_D() < 0) {
        h_curr = h_curr.MUL_ZM_Z();
        k_curr = k_curr.MUL_ZM_Z();
    }
    
    Rational result(h_curr, k_curr.ABS_Z_N());
    result.RED_Q_Q();
    return result;
}

// CF-5 | Получение приближения с заданной точностью
Rational ContinuedFraction::APPROX_CF_Q(const Natural& max_denominator) const {
    if (coefficients_.empty()) {
        return Rational();
    }
    
    Integer h_prev = Integer("1");
    Integer k_prev = Integer("0");
    Integer h_curr = coefficients_[0];
    Integer k_curr = Integer("1");
    
    Rational best_approx(h_curr, k_curr.ABS_Z_N());
    best_approx.RED_Q_Q();
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        Integer h_new = coefficients_[i].MUL_ZZ_Z(h_curr).ADD_ZZ_Z(h_prev);
        Integer k_new = coefficients_[i].MUL_ZZ_Z(k_curr).ADD_ZZ_Z(k_prev);
        
        // проверяем, не превышает ли знаменатель максимум
        Natural abs_k = k_new.ABS_Z_N();
        if (abs_k.COM_NN_D(max_denominator) == 2) {
            break;
        }
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
        
        if (k_curr.SGN_Z_D() < 0) {
            h_curr = h_curr.MUL_ZM_Z();
            k_curr = k_curr.MUL_ZM_Z();
        }
        
        best_approx = Rational(h_curr, k_curr.ABS_Z_N());
        best_approx.RED_Q_Q();
    }
    
    return best_approx;
}

// CF-6 | Сравнение двух цепных дробей
int ContinuedFraction::COM_CF_D(const ContinuedFraction& other) const {
    size_t min_len = std::min(coefficients_.size(), other.coefficients_.size());
    
    for (size_t i = 0; i < min_len; ++i) {
        int cmp = coefficients_[i].COM_ZZ_D(other.coefficients_[i]);
        if (cmp != 0) {
            // Для чётных позиций: больший коэффициент = большая дробь
            // Для нечётных позиций: больший коэффициент = меньшая дробь
            if (i % 2 == 0) {
                return (cmp == 2) ? 1 : -1;
            } else {
                return (cmp == 2) ? -1 : 1;
            }
        }
    }
    
    if (coefficients_.size() == other.coefficients_.size()) {
        return 0;
    }
    
    if (coefficients_.size() > other.coefficients_.size()) {
        return (min_len % 2 == 0) ? 1 : -1;
    } else {
        return (min_len % 2 == 0) ? -1 : 1;
    }
}

// CF-7 | Получение обратной дроби (переворот)
ContinuedFraction ContinuedFraction::INV_CF_CF() const {
    if (coefficients_.empty()) {
        throw std::runtime_error("Невозможно инвертировать пустую цепную дробь");
    }
    
    // Проверяем, что дробь не равна нулю
    Rational val = TO_CF_Q();
    if (!val.NZER_Q_B()) {
        throw std::runtime_error("Невозможно инвертировать нулевую дробь");
    }
    
    std::vector<Integer> new_coeffs;

    if (coefficients_[0].SGN_Z_D() == 0) {
        for (size_t i = 1; i < coefficients_.size(); ++i) {
            new_coeffs.push_back(coefficients_[i]);
        }
    } else {
        new_coeffs.push_back(Integer("0"));
        for (const auto& c : coefficients_) {
            new_coeffs.push_back(c);
        }
    }
    
    return ContinuedFraction(new_coeffs);
}


// CF-8 | Преобразование sqrt(D) в периодическую цепную дробь
ContinuedFraction ContinuedFraction::FROM_SQRT_CF(const Natural& D) {
    // проверяем, что D не является полным квадратом, находим floor(sqrt(D))
    Natural one("1");
    Natural two("2");
    
    // бинарный поиск для floor(sqrt(D))
    Natural low("0");
    Natural high = D;
    Natural a0("0");
    
    while (low.COM_NN_D(high) != 2) {  // low <= high
        Natural mid = low.ADD_NN_N(high).DIV_NN_N(two);
        Natural mid_sq = mid.MUL_NN_N(mid);
        
        int cmp = mid_sq.COM_NN_D(D);
        if (cmp == 0) {
            // D - полный квадрат
            return ContinuedFraction({Integer(mid.as_string())});
        } else if (cmp == 1) {  // mid^2 < D
            a0 = mid;
            low = mid.ADD_NN_N(one);
        } else {  // mid^2 > D
            if (mid.COM_NN_D(one) != 2) break;  // mid >= 1
            high = mid.SUB_NN_N(one);
        }
    }
    
    // a0 = floor(sqrt(D))
    std::vector<Integer> coeffs;
    coeffs.push_back(Integer(a0.as_string()));
    
    // Алгоритм для периодической части
    // (m_n + sqrt(D)) / d_n, начинаем с m_0 = 0, d_0 = 1
    Integer m = Integer("0");
    Integer d = Integer("1");
    Integer a0_int = Integer(a0.as_string());
    
    // Для обнаружения периода храним пары (m, d)
    std::vector<std::pair<std::string, std::string>> states;
    int period_start = -1;
    
    // Максимум итераций для безопасности
    const int MAX_ITER = 1000;
    
    for (int iter = 0; iter < MAX_ITER; ++iter) {
        // m_{n+1} = d_n * a_n - m_n
        Integer a_n = coeffs.back();
        m = d.MUL_ZZ_Z(a_n).SUB_ZZ_Z(m);
        
        // d_{n+1} = (D - m_{n+1}^2) / d_n
        Integer m_sq = m.MUL_ZZ_Z(m);
        Integer D_int = Integer(D.as_string());
        d = D_int.SUB_ZZ_Z(m_sq).DIV_ZZ_Z(d);
        
        if (d.SGN_Z_D() == 0) break;
        
        // a_{n+1} = floor((a0 + m_{n+1}) / d_{n+1})
        Integer a_next = a0_int.ADD_ZZ_Z(m).DIV_ZZ_Z(d);
        
        // Проверяем на период
        std::string state_key = m.as_string() + "," + d.as_string();
        for (size_t i = 0; i < states.size(); ++i) {
            if (states[i].first == m.as_string() && states[i].second == d.as_string()) {
                period_start = static_cast<int>(i) + 1;  // +1 потому что a0 уже добавлен
                return ContinuedFraction(coeffs, period_start);
            }
        }
        states.push_back({m.as_string(), d.as_string()});
        
        coeffs.push_back(a_next);
        
        // Период sqrt(D) всегда заканчивается на 2*a0
        if (a_next.COM_ZZ_D(a0_int.MUL_ZZ_Z(Integer("2"))) == 0) {
            period_start = 1;  // Период начинается после a0
            return ContinuedFraction(coeffs, period_start);
        }
    }
    
    return ContinuedFraction(coeffs, period_start);
}

// CF-9 | Преобразование (a + sqrt(D)) / c в периодическую цепную дробь
ContinuedFraction ContinuedFraction::FROM_QUAD_CF(const Integer& a, const Natural& D, const Integer& c) {
    if (c.SGN_Z_D() == 0) {
        throw std::invalid_argument("Знаменатель не может быть нулём");
    }
    
    // Находим floor(sqrt(D))
    Natural one("1");
    Natural two("2");
    Natural low("0");
    Natural high = D;
    Natural sqrt_D("0");
    
    while (low.COM_NN_D(high) != 2) {
        Natural mid = low.ADD_NN_N(high).DIV_NN_N(two);
        Natural mid_sq = mid.MUL_NN_N(mid);
        
        int cmp = mid_sq.COM_NN_D(D);
        if (cmp == 0) {
            sqrt_D = mid;
            break;
        } else if (cmp == 1) {
            sqrt_D = mid;
            low = mid.ADD_NN_N(one);
        } else {
            if (mid.COM_NN_D(one) != 2) break;
            high = mid.SUB_NN_N(one);
        }
    }
    
    Integer sqrt_D_int = Integer(sqrt_D.as_string());
    
    std::vector<Integer> coeffs;
    
    // Текущее состояние: (m + sqrt(D)) / d
    Integer m = a;
    Integer d = c;
    
    // Нормализуем знак знаменателя
    if (d.SGN_Z_D() < 0) {
        m = m.MUL_ZM_Z();
        d = d.MUL_ZM_Z();
    }
    
    std::vector<std::pair<std::string, std::string>> states;
    int period_start = -1;
    const int MAX_ITER = 1000;
    
    for (int iter = 0; iter < MAX_ITER; ++iter) {
        // a_n = floor((m + sqrt(D)) / d)
        Integer a_n = m.ADD_ZZ_Z(sqrt_D_int).DIV_ZZ_Z(d);
        coeffs.push_back(a_n);
        
        // Проверяем на период
        std::string state_key = m.as_string() + "," + d.as_string();
        for (size_t i = 0; i < states.size(); ++i) {
            if (states[i].first == m.as_string() && states[i].second == d.as_string()) {
                period_start = static_cast<int>(i);
                coeffs.pop_back();  // Убираем дублирующийся элемент
                return ContinuedFraction(coeffs, period_start);
            }
        }
        states.push_back({m.as_string(), d.as_string()});
        
        // m_{n+1} = d_n * a_n - m_n
        m = d.MUL_ZZ_Z(a_n).SUB_ZZ_Z(m);
        
        // d_{n+1} = (D - m_{n+1}^2) / d_n
        Integer m_sq = m.MUL_ZZ_Z(m);
        Integer D_int = Integer(D.as_string());
        d = D_int.SUB_ZZ_Z(m_sq).DIV_ZZ_Z(d);
        
        if (d.SGN_Z_D() == 0) break;
    }
    
    return ContinuedFraction(coeffs, period_start);
}

// CF-10 | Проверка, является ли дробь периодической
bool ContinuedFraction::IS_PERIODIC_CF() const {
    return period_start_ >= 0;
}

// CF-11 | Получение периода
std::vector<Integer> ContinuedFraction::GET_PERIOD_CF() const {
    if (!IS_PERIODIC_CF()) {
        return {};
    }
    
    std::vector<Integer> period;
    for (size_t i = period_start_; i < coefficients_.size(); ++i) {
        period.push_back(coefficients_[i]);
    }
    return period;
}

// CF-12 | Получение непериодической части
std::vector<Integer> ContinuedFraction::GET_APERIODIC_CF() const {
    std::vector<Integer> aperiodic;
    int end = IS_PERIODIC_CF() ? period_start_ : static_cast<int>(coefficients_.size());
    for (int i = 0; i < end; ++i) {
        aperiodic.push_back(coefficients_[i]);
    }
    return aperiodic;
}

// Строковое представление
std::string ContinuedFraction::as_string() const {
    if (coefficients_.empty()) {
        return "[0]";
    }
    
    std::ostringstream oss;
    oss << "[" << coefficients_[0].as_string();
    
    if (coefficients_.size() > 1) {
        oss << "; ";
        
        for (size_t i = 1; i < coefficients_.size(); ++i) {
            if (IS_PERIODIC_CF() && static_cast<int>(i) == period_start_) {
                oss << "(";
            }
            
            if (i > 1) oss << ", ";
            oss << coefficients_[i].as_string();
        }
        
        if (IS_PERIODIC_CF()) {
            oss << ")...";
        }
    }
    
    oss << "]";
    return oss.str();
}

// Проверка на равенство
bool ContinuedFraction::EQ_CF_B(const ContinuedFraction& other) const {
    return COM_CF_D(other) == 0;
}

std::ostream& operator<<(std::ostream& os, const ContinuedFraction& cf) {
    return os << cf.as_string();
}

bool operator==(const ContinuedFraction& cf1, const ContinuedFraction& cf2) {
    return cf1.EQ_CF_B(cf2);
}

bool operator<(const ContinuedFraction& cf1, const ContinuedFraction& cf2) {
    return cf1.COM_CF_D(cf2) < 0;
}

bool operator>(const ContinuedFraction& cf1, const ContinuedFraction& cf2) {
    return cf1.COM_CF_D(cf2) > 0;
}


PolynomialContinuedFraction::PolynomialContinuedFraction() 
    : coefficients_({Polynomial()}) {}

PolynomialContinuedFraction::PolynomialContinuedFraction(const std::vector<Polynomial>& coefficients)
    : coefficients_(coefficients) {
    if (coefficients_.empty()) {
        coefficients_.push_back(Polynomial());
    }
}

// PCF-1 | Преобразование рациональной функции P/Q в цепную дробь
// Алгоритм Евклида для многочленов
PolynomialContinuedFraction PolynomialContinuedFraction::FROM_PQ_PCF(const Polynomial& P, const Polynomial& Q) {
    if (!Q.NZER_P_B()) {
        throw std::invalid_argument("Знаменатель не может быть нулевым многочленом");
    }
    
    std::vector<Polynomial> coeffs;
    
    Polynomial p = P;
    Polynomial q = Q;
    
    // Алгоритм Евклида: a_i = p div q, затем p, q = q, p mod q
    while (q.NZER_P_B()) {
        Polynomial quotient = p.DIV_PP_P(q);
        coeffs.push_back(quotient);
        
        Polynomial remainder = p.MOD_PP_P(q);
        
        p = q;
        q = remainder;
    }
    
    return PolynomialContinuedFraction(coeffs);
}

// PCF-2 | Преобразование цепной дроби в рациональную функцию
std::pair<Polynomial, Polynomial> PolynomialContinuedFraction::TO_PCF_PQ() const {
    if (coefficients_.empty()) {
        return {Polynomial(), Polynomial({{0, "1"}})};
    }
    
    // Рекуррентное соотношение для числителя и знаменателя
    // h_{-1} = 1, h_0 = a_0
    // k_{-1} = 0, k_0 = 1
    // h_n = a_n * h_{n-1} + h_{n-2}
    // k_n = a_n * k_{n-1} + k_{n-2}
    
    Polynomial h_prev({{0, "1"}});  // h_{-1} = 1
    Polynomial k_prev;               // k_{-1} = 0
    Polynomial h_curr = coefficients_[0];  // h_0 = a_0
    Polynomial k_curr({{0, "1"}});  // k_0 = 1
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        Polynomial h_new = coefficients_[i].MUL_PP_P(h_curr).ADD_PP_P(h_prev);
        Polynomial k_new = coefficients_[i].MUL_PP_P(k_curr).ADD_PP_P(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
    }
    
    return {h_curr, k_curr};
}

// PCF-3 | Получение последовательности конвергентов
std::vector<std::pair<Polynomial, Polynomial>> PolynomialContinuedFraction::CONVERGENTS_PCF() const {
    std::vector<std::pair<Polynomial, Polynomial>> convergents;
    
    if (coefficients_.empty()) {
        return convergents;
    }
    
    Polynomial h_prev({{0, "1"}});
    Polynomial k_prev;
    Polynomial h_curr = coefficients_[0];
    Polynomial k_curr({{0, "1"}});
    
    convergents.push_back({h_curr, k_curr});
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        Polynomial h_new = coefficients_[i].MUL_PP_P(h_curr).ADD_PP_P(h_prev);
        Polynomial k_new = coefficients_[i].MUL_PP_P(k_curr).ADD_PP_P(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
        
        convergents.push_back({h_curr, k_curr});
    }
    
    return convergents;
}

// PCF-4 | Получение n-го конвергента
std::pair<Polynomial, Polynomial> PolynomialContinuedFraction::CONVERGENT_N_PCF(int n) const {
    if (n < 0 || n >= static_cast<int>(coefficients_.size())) {
        throw std::out_of_range("Индекс конвергента вне диапазона");
    }
    
    Polynomial h_prev({{0, "1"}});
    Polynomial k_prev;
    Polynomial h_curr = coefficients_[0];
    Polynomial k_curr({{0, "1"}});
    
    for (int i = 1; i <= n; ++i) {
        Polynomial h_new = coefficients_[i].MUL_PP_P(h_curr).ADD_PP_P(h_prev);
        Polynomial k_new = coefficients_[i].MUL_PP_P(k_curr).ADD_PP_P(k_prev);
        
        h_prev = h_curr;
        k_prev = k_curr;
        h_curr = h_new;
        k_curr = k_new;
    }
    
    return {h_curr, k_curr};
}

// PCF-5 | Сравнение двух цепных дробей многочленов (по степени результата)
int PolynomialContinuedFraction::COM_PCF_D(const PolynomialContinuedFraction& other) const {
    auto [p1, q1] = TO_PCF_PQ();
    auto [p2, q2] = other.TO_PCF_PQ();
    
    // сравниваем по степени числителя минус степень знаменателя
    int deg1 = static_cast<int>(std::stoi(p1.DEG_P_N().as_string())) - 
               static_cast<int>(std::stoi(q1.DEG_P_N().as_string()));
    int deg2 = static_cast<int>(std::stoi(p2.DEG_P_N().as_string())) - 
               static_cast<int>(std::stoi(q2.DEG_P_N().as_string()));
    
    if (deg1 < deg2) return -1;
    if (deg1 > deg2) return 1;
    return 0;
}

// PCF-6 | Получение обратной дроби
PolynomialContinuedFraction PolynomialContinuedFraction::INV_PCF_PCF() const {
    if (coefficients_.empty()) {
        throw std::runtime_error("Невозможно инвертировать пустую цепную дробь");
    }

    auto [p, q] = TO_PCF_PQ();
    if (!p.NZER_P_B()) {
        throw std::runtime_error("Невозможно инвертировать нулевую дробь");
    }
    
    std::vector<Polynomial> new_coeffs;

    if (!coefficients_[0].NZER_P_B()) {
        for (size_t i = 1; i < coefficients_.size(); ++i) {
            new_coeffs.push_back(coefficients_[i]);
        }
    } else {
        new_coeffs.push_back(Polynomial());
        for (const auto& c : coefficients_) {
            new_coeffs.push_back(c);
        }
    }
    
    return PolynomialContinuedFraction(new_coeffs);
}

std::string PolynomialContinuedFraction::as_string() const {
    if (coefficients_.empty()) {
        return "[0]";
    }
    
    std::ostringstream oss;
    oss << "[(" << coefficients_[0].as_string() << ")";
    
    if (coefficients_.size() > 1) {
        oss << "; ";
        
        for (size_t i = 1; i < coefficients_.size(); ++i) {
            if (i > 1) oss << ", ";
            oss << "(" << coefficients_[i].as_string() << ")";
        }
    }
    
    oss << "]";
    return oss.str();
}

bool PolynomialContinuedFraction::EQ_PCF_B(const PolynomialContinuedFraction& other) const {
    if (coefficients_.size() != other.coefficients_.size()) {
        return false;
    }
    
    for (size_t i = 0; i < coefficients_.size(); ++i) {
        if (!coefficients_[i].EQ_PP_B(other.coefficients_[i])) {
            return false;
        }
    }
    
    return true;
}

std::ostream& operator<<(std::ostream& os, const PolynomialContinuedFraction& pcf) {
    return os << pcf.as_string();
}

bool operator==(const PolynomialContinuedFraction& pcf1, const PolynomialContinuedFraction& pcf2) {
    return pcf1.EQ_PCF_B(pcf2);
}
