#include "continued_fraction.hpp"
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <set>
#include <tuple>


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
        q = remainder.ABS_Z_N();
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
            // COM_ZZ_D возвращает: 1 если this > other, -1 если this < other
            if (i % 2 == 0) {
                return cmp;  // на чётной позиции порядок прямой
            } else {
                return -cmp;  // на нечётной позиции порядок обратный
            }
        }
    }
    
    if (coefficients_.size() == other.coefficients_.size()) {
        return 0;
    }
    
    // Все общие элементы равны, но длины разные
    // Более короткая дробь эквивалентна дроби с "бесконечностью" на позиции min_len
    // На позиции min_len (чётная): больший элемент = большая дробь, ∞ > любого числа => короткая > длинной
    // На позиции min_len (нечётная): больший элемент = меньшая дробь, ∞ > любого числа => короткая < длинной
    if (coefficients_.size() > other.coefficients_.size()) {
        // this длиннее, other короче (other имеет "∞" на позиции min_len)
        return (min_len % 2 == 0) ? -1 : 1;
    } else {
        // other длиннее, this короче (this имеет "∞" на позиции min_len)
        return (min_len % 2 == 0) ? 1 : -1;
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

// CF-13 | Преобразование периодической цепной дроби в квадратичную иррациональность
// Возвращает (a, D, c) такие что дробь = (a + sqrt(D)) / c
std::tuple<Integer, Natural, Integer> ContinuedFraction::TO_CF_QUAD() const {
    // Если дробь не периодическая, возвращаем рациональное число
    if (!IS_PERIODIC_CF()) {
        Rational r = TO_CF_Q();
        return {r.numerator(), Natural("0"), Integer(r.denominator().as_string())};
    }
    
    // Получаем период и непериодическую часть
    std::vector<Integer> period = GET_PERIOD_CF();
    std::vector<Integer> aperiodic = GET_APERIODIC_CF();
    
    if (period.empty()) {
        Rational r = TO_CF_Q();
        return {r.numerator(), Natural("0"), Integer(r.denominator().as_string())};
    }
    
    // Шаг 1: Вычисляем конвергенты периода [b1; b2, ..., bk]
    // Для уравнения x = (p_k + p_{k-1} * x) / (q_k + q_{k-1} * x)
    // где x = [b1; b2, ..., bk, x] - чисто периодическая часть
    
    Integer p_prev = Integer("1");   // p_{-1} = 1
    Integer q_prev = Integer("0");   // q_{-1} = 0
    Integer p_curr = period[0];      // p_0 = b_1
    Integer q_curr = Integer("1");   // q_0 = 1
    
    for (size_t i = 1; i < period.size(); ++i) {
        Integer p_new = period[i].MUL_ZZ_Z(p_curr).ADD_ZZ_Z(p_prev);
        Integer q_new = period[i].MUL_ZZ_Z(q_curr).ADD_ZZ_Z(q_prev);
        
        p_prev = p_curr;
        q_prev = q_curr;
        p_curr = p_new;
        q_curr = q_new;
    }
    
    // p_k = p_curr, p_{k-1} = p_prev
    // q_k = q_curr, q_{k-1} = q_prev
    
    // Шаг 2: Решаем квадратное уравнение для чисто периодической части
    // x = [b1; b2, ..., bk, x]
    // x = b1 + 1/(b2 + 1/(...+ 1/(bk + 1/x)))
    // Используя конвергенты: x = (p_k * x + p_{k-1}) / (q_k * x + q_{k-1})
    // => x * (q_k * x + q_{k-1}) = p_k * x + p_{k-1}
    // => q_k * x^2 + q_{k-1} * x = p_k * x + p_{k-1}
    // => q_k * x^2 + (q_{k-1} - p_k) * x - p_{k-1} = 0
    
    Integer coef_a = q_curr;  // q_k
    Integer coef_b = q_prev.SUB_ZZ_Z(p_curr);  // q_{k-1} - p_k
    Integer coef_c = p_prev.MUL_ZM_Z();  // -p_{k-1}
    
    // D = b^2 - 4ac = (q_{k-1} - p_k)^2 + 4 * q_k * p_{k-1}
    Integer b_sq = coef_b.MUL_ZZ_Z(coef_b);
    Integer four = Integer("4");
    Integer four_ac = four.MUL_ZZ_Z(coef_a).MUL_ZZ_Z(p_prev);
    Integer D_int = b_sq.ADD_ZZ_Z(four_ac);
    
    // D должен быть неотрицательным
    if (D_int.SGN_Z_D() < 0) {
        Rational r = TO_CF_Q();
        return {r.numerator(), Natural("0"), Integer(r.denominator().as_string())};
    }
    
    Natural D_nat = D_int.ABS_Z_N();
    
    // x = (-b + sqrt(D)) / (2a) = (p_k - q_{k-1} + sqrt(D)) / (2 * q_k)
    Integer x_num = coef_b.MUL_ZM_Z();  // -coef_b = p_k - q_{k-1}
    Integer x_den = Integer("2").MUL_ZZ_Z(coef_a);  // 2 * q_k
    
    // Если нет непериодической части, результат = x
    if (aperiodic.empty()) {
        // Нормализуем знак знаменателя
        if (x_den.SGN_Z_D() < 0) {
            x_num = x_num.MUL_ZM_Z();
            x_den = x_den.MUL_ZM_Z();
        }
        return {x_num, D_nat, x_den};
    }
    
    // Шаг 3: Учитываем непериодическую часть
    // result = [a0; a1, ..., a_{m-1}, x]
    // result = (P_{m-1} * x + P_{m-2}) / (Q_{m-1} * x + Q_{m-2})
    // где P_i/Q_i - конвергенты непериодической части
    
    Integer P_prev = Integer("1");   // P_{-1} = 1
    Integer Q_prev = Integer("0");   // Q_{-1} = 0
    Integer P_curr = aperiodic[0];   // P_0 = a_0
    Integer Q_curr = Integer("1");   // Q_0 = 1
    
    for (size_t i = 1; i < aperiodic.size(); ++i) {
        Integer P_new = aperiodic[i].MUL_ZZ_Z(P_curr).ADD_ZZ_Z(P_prev);
        Integer Q_new = aperiodic[i].MUL_ZZ_Z(Q_curr).ADD_ZZ_Z(Q_prev);
        
        P_prev = P_curr;
        Q_prev = Q_curr;
        P_curr = P_new;
        Q_curr = Q_new;
    }
    
    // P_{m-1} = P_curr, P_{m-2} = P_prev
    // Q_{m-1} = Q_curr, Q_{m-2} = Q_prev
    
    // result = (P_{m-1} * x + P_{m-2}) / (Q_{m-1} * x + Q_{m-2})
    // где x = (x_num + sqrt(D)) / x_den
    //
    // Подставляем:
    // числитель = P_{m-1} * (x_num + sqrt(D)) / x_den + P_{m-2}
    //           = (P_{m-1} * x_num + P_{m-2} * x_den + P_{m-1} * sqrt(D)) / x_den
    //           = (A + B * sqrt(D)) / x_den
    // где A = P_{m-1} * x_num + P_{m-2} * x_den, B = P_{m-1}
    //
    // знаменатель = Q_{m-1} * (x_num + sqrt(D)) / x_den + Q_{m-2}
    //             = (Q_{m-1} * x_num + Q_{m-2} * x_den + Q_{m-1} * sqrt(D)) / x_den
    //             = (C + E * sqrt(D)) / x_den
    // где C = Q_{m-1} * x_num + Q_{m-2} * x_den, E = Q_{m-1}
    //
    // result = (A + B * sqrt(D)) / (C + E * sqrt(D))
    //
    // Рационализируем знаменатель:
    // result = (A + B * sqrt(D)) * (C - E * sqrt(D)) / ((C + E * sqrt(D)) * (C - E * sqrt(D)))
    //        = (A*C - B*E*D + (B*C - A*E) * sqrt(D)) / (C^2 - E^2 * D)
    
    Integer A = P_curr.MUL_ZZ_Z(x_num).ADD_ZZ_Z(P_prev.MUL_ZZ_Z(x_den));
    Integer B = P_curr;
    Integer C = Q_curr.MUL_ZZ_Z(x_num).ADD_ZZ_Z(Q_prev.MUL_ZZ_Z(x_den));
    Integer E = Q_curr;
    
    // числитель: (A*C - B*E*D) + (B*C - A*E) * sqrt(D)
    Integer D_as_int = Integer(D_nat.as_string());
    Integer AC = A.MUL_ZZ_Z(C);
    Integer BED = B.MUL_ZZ_Z(E).MUL_ZZ_Z(D_as_int);
    Integer result_const = AC.SUB_ZZ_Z(BED);  // A*C - B*E*D
    
    Integer BC = B.MUL_ZZ_Z(C);
    Integer AE = A.MUL_ZZ_Z(E);
    Integer result_sqrt_coef = BC.SUB_ZZ_Z(AE);  // B*C - A*E
    
    // знаменатель: C^2 - E^2 * D
    Integer C_sq = C.MUL_ZZ_Z(C);
    Integer E_sq_D = E.MUL_ZZ_Z(E).MUL_ZZ_Z(D_as_int);
    Integer result_den = C_sq.SUB_ZZ_Z(E_sq_D);
    
    // Результат: (result_const + result_sqrt_coef * sqrt(D)) / result_den
    
    // Упрощаем: делим всё на result_sqrt_coef (если он не 0)
    if (result_sqrt_coef.SGN_Z_D() == 0) {
        // Нет sqrt(D) в результате - это рациональное число
        if (result_den.SGN_Z_D() < 0) {
            result_const = result_const.MUL_ZM_Z();
            result_den = result_den.MUL_ZM_Z();
        }
        // Сокращаем
        Natural gcd = result_const.ABS_Z_N().GCF_NN_N(result_den.ABS_Z_N());
        if (gcd.NZER_N_B() && gcd.COM_NN_D(Natural("1")) != 0) {
            result_const = result_const.DIV_ZZ_Z(Integer(gcd.as_string()));
            result_den = result_den.DIV_ZZ_Z(Integer(gcd.as_string()));
        }
        return {result_const, Natural("0"), result_den};
    }
    
    // Находим НОД всех трёх коэффициентов
    Natural abs_const = result_const.ABS_Z_N();
    Natural abs_coef = result_sqrt_coef.ABS_Z_N();
    Natural abs_den = result_den.ABS_Z_N();
    
    Natural gcd = abs_const.GCF_NN_N(abs_coef);
    gcd = gcd.GCF_NN_N(abs_den);
    
    if (gcd.NZER_N_B() && gcd.COM_NN_D(Natural("1")) != 0) {
        result_const = result_const.DIV_ZZ_Z(Integer(gcd.as_string()));
        result_sqrt_coef = result_sqrt_coef.DIV_ZZ_Z(Integer(gcd.as_string()));
        result_den = result_den.DIV_ZZ_Z(Integer(gcd.as_string()));
    }
    
    // Нормализуем знак знаменателя
    if (result_den.SGN_Z_D() < 0) {
        result_const = result_const.MUL_ZM_Z();
        result_sqrt_coef = result_sqrt_coef.MUL_ZM_Z();
        result_den = result_den.MUL_ZM_Z();
    }
    
    // Если коэффициент при sqrt(D) отрицательный, меняем знаки
    if (result_sqrt_coef.SGN_Z_D() < 0) {
        result_const = result_const.MUL_ZM_Z();
        result_sqrt_coef = result_sqrt_coef.MUL_ZM_Z();
        result_den = result_den.MUL_ZM_Z();
    }
    
    // D_new = k^2 * D, где k = result_sqrt_coef
    Natural k_nat = result_sqrt_coef.ABS_Z_N();
    Natural D_new = k_nat.MUL_NN_N(k_nat).MUL_NN_N(D_nat);
    
    return {result_const, D_new, result_den};
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
    
    // выносим общий рациональный множитель
    // FAC_P_Q возвращает (множитель, нормализованный многочлен с целыми коэффициентами)
    if (h_curr.NZER_P_B()) {
        auto [fac_h, norm_h] = h_curr.FAC_P_Q();
        h_curr = norm_h;
    }
    if (k_curr.NZER_P_B()) {
        auto [fac_k, norm_k] = k_curr.FAC_P_Q();
        k_curr = norm_k;
    }
    
    // сокращаем на НОД
    if (h_curr.NZER_P_B() && k_curr.NZER_P_B()) {
        Polynomial gcd = h_curr.GCF_PP_P(k_curr);
        if (gcd.NZER_P_B()) {
            Polynomial one({{0, "1"}});
            if (!gcd.EQ_PP_B(one)) {
                h_curr = h_curr.DIV_PP_P(gcd);
                k_curr = k_curr.DIV_PP_P(gcd);
            }
        }
    }
    
    // знак
    if (k_curr.NZER_P_B()) {
        Rational k_lead = k_curr.LED_P_Q();
        if (k_lead.numerator().SGN_Z_D() < 0) {
            h_curr = h_curr.MUL_PQ_P(Rational("-1"));
            k_curr = k_curr.MUL_PQ_P(Rational("-1"));
        }
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
