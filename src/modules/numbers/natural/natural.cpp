#include "natural.hpp"
    #include <iostream>
Natural::Natural(): n_(0), a_({'0'}) {};

Natural::Natural(std::string s) {
    int first_not_zero = 0;

    // находим первую не нулевую цифру, либо единственный ноль
    while (first_not_zero < s.size() - 1 && s[first_not_zero] == '0') {
        first_not_zero++;
    }

    // сохраняем массив цифр
    for (int i = first_not_zero; i < s.size(); i++) {
        if (isdigit(s[i])) {
            a_.push_back(s[i]);
        }
        else {
            throw std::invalid_argument("Строка должна содержать только цифры!");
        }
    }

    // для удобства вычислений переворачиваем

    std::reverse(a_.begin(), a_.end());

    n_ = a_.size() - 1;
};

std::string Natural::as_string() const {
    std::string str = a_;

    std::reverse(str.begin(), str.end());

    return str;
}


//N-1
int Natural::COM_NN_D(const Natural& other) const {
    if (n_ > other.n_) {
        return 2; 
    } else if (n_ < other.n_){
        return 1; 
    }
    //  если количество разрядов равно то сравниваем
    //  поразрядно, начиная со старшего
    //  если ни разу не сработал return - числа равны
    for (int i = n_; i >= 0; --i) {
        if ((a_[i] - '0') > (other.a_[i] - '0')) {
            return 2; 
        } else if ((a_[i] - '0') < (other.a_[i] - '0')) {
            return 1; 
        }
    }
    return 0;
}

// N-2
bool Natural::NZER_N_B() const {

    std::cout << n_ << ' ' << a_ << '\n';
    return this->a_[n_] - '0';
}

// N-3
Natural Natural::ADD_1N_N() {
    return ADD_NN_N(Natural("1"));
}

// N-4
Natural Natural::ADD_NN_N(const Natural &other) const {
    Natural smaller = *this;
    Natural larger = other;
    if (COM_NN_D(other) != 1)
        std::swap(smaller, larger);

    std::cout << "smaller=" << smaller << ' ' << larger << '\n';

    int transfer = 0;

    std::string result = larger.a_;

    for (int i = 0; i <= larger.n_; i++) {
        int dig_smaller = (i <= smaller.n_) ? (smaller.a_[i] - '0') : 0;
        int dig_larger = larger.a_[i] - '0';

        int digit = dig_larger + dig_smaller + transfer;
        transfer = digit / 10;
        result[i] = (digit % 10) + '0';
    }

    if (transfer > 0) {
        result.push_back(transfer + '0');
    }

    std::reverse(result.begin(), result.end());

    return Natural(result);
}

// N-5
Natural Natural::SUB_NN_N(const Natural &other) const
{
    if (COM_NN_D(other) == 1) {
        throw std::invalid_argument("Вычитаемое больше уменьшаемого");
    }

    if (COM_NN_D(other) == 0) {
        return Natural();
    }

    std::string result = as_string();
    int borrow = 0;

    for (int i = 0; i <= n_; i++) {
        int dig1 = (a_[i] - '0');
        int dig2 = (i <= other.n_) ? (other.a_[i] - '0') : 0;

        dig1 -= borrow;

        if (dig1 < dig2) {
            dig1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        int diff = dig1 - dig2;
        result[i] = (diff + '0');
    }

    std::reverse(result.begin(), result.end());

    return Natural(result);
}

// N-6
Natural Natural::MUL_ND_N(int digit) const
{
    if (digit > 10 || digit < 0) {
        throw std::invalid_argument("Множитель не может быть отрицательным и >= 9");
    }

    if (digit == 0 || (n_ == 0 && a_[0] == '0'))
        return Natural();
    if (digit == 1)
        return *this;

    std::string result;
    int transfer = 0;
    int i = 0;

    for (; i < n_; i++) {
        int num = (a_[i] - '0') * digit + transfer;
        transfer = num / 10;
        char dig = (num % 10) + '0';
        result += dig;
    }

    if (transfer > 0) {
        char dig = transfer + '0';
        result += dig;
    }

    reverse(result.begin(), result.end());

    return Natural(result);
}

// N-7
Natural Natural::MUL_Nk_N(int k) const {
    if (n_ == 0 && a_[0] == '0')
        return Natural();

    std::string result = as_string();

    for (int i = 0; i < k; i++) {
        result += '0';
    }

    return Natural(result);
}

// N-8
Natural Natural::MUL_NN_N(const Natural &other) const {
    Natural result;

    for (int i = 0; i < other.n_; i++) {
        // переменная, которая будет содержать результат умножения
        // числа на текущую цифру другого числа с поправкой на разряд цифры
        Natural step = this->MUL_ND_N(other.a_[i] - '0'); // умножаем число на текущую цифру другого числа

        // домножаем на 10^i
        step = step.MUL_Nk_N(i);

        // складываем результат, полученный на текущем шаге, с общим результатом
        result = result.ADD_NN_N(step);
    }

    return result;
}

// N-9
Natural Natural::SUB_NDN_N(const Natural &other, int digit) const {
    Natural subtrahend = other.MUL_ND_N(digit);
    Natural result = SUB_NN_N(subtrahend);

    return result;
}

// N-10
std::pair<int, int> Natural::DIV_NN_Dk(const Natural &other) const {
    int k = 0;
    while (COM_NN_D(other.MUL_Nk_N(k)) != 1) {
        k++; // находим такое k, что другое, умноженное на 10^k превышает
             // наше число и берем предыдущее значение k
    }
    k--;

    Natural mul_k = other.MUL_Nk_N(k);

    int dig = 1;
    for (; dig < 10; dig++) {
        Natural tmp = mul_k.MUL_ND_N(dig); // идем по цифрам
        if (COM_NN_D(tmp) == 1)
            break;
    }
    dig--;

    return std::make_pair(dig, k);
}

// N-11
Natural Natural::DIV_NN_N(const Natural &other) const {
    // если делитель не равен 0
    if (!other.NZER_N_B())
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");
    // для сохранения результата
    Natural result;
    // для хранения текущего делимого
    Natural current_num = *this;

    // пока текущее делимое не стане меньше делителя
    while (current_num.COM_NN_D(other) != 1) {
        // получаем первую цифру (digit) неполного частного от деления
        // текущего делимого на делитель и ее разряд (k)
        std::pair<int, int> digit_k = current_num.DIV_NN_Dk(other);

        // делаем поправку на разряд первой цифры неполного частного
        // и прибавляем полученное число к результату
        Natural adjusted_result = Natural(std::to_string(digit_k.first)).MUL_Nk_N(digit_k.second);
        result = result.ADD_NN_N(adjusted_result);

        // делаем поправку на разряд первой цифры неполного частного
        // и вычитаем, умножив на делитель, из текущего делимого
        Natural adjusted_current = Natural(other).MUL_Nk_N(digit_k.second);
        current_num = current_num.SUB_NDN_N(adjusted_current, digit_k.first);
    }

    return result;
}

// N-12
Natural Natural::MOD_NN_N(const Natural &other) const {
    // если делитель не равен 0
    if (!other.NZER_N_B())
        throw std::invalid_argument("Делитель должен быть отличен от нуля!");

    // получаем неполное частное от деления
    Natural division_result = this->DIV_NN_N(other);
    // вычитаем из исходного числа неполное частное,
    // умноженное на делитель
    Natural result = this->SUB_NN_N(division_result.MUL_NN_N(other));

    return result;
}

// N-13
Natural Natural::GCF_NN_N(const Natural &other) const {
    // Алгоритм Евклида для нахождения НОД

    // Начальные значения
    Natural a = *this;
    Natural b = other;

    // Пока a и b больше 0
    while (a.NZER_N_B() && b.NZER_N_B()) {
        // Берем остаток от деления большего на меньшее
        if (a.COM_NN_D(b) != 1)
            a = a.MOD_NN_N(b);
        else
            b = b.MOD_NN_N(a);
    }

    // После завершения цикла выбираем максимальное из двух оставшихся чисел
    Natural max;
    if (a.COM_NN_D(b) != 1)
        max = a;
    else
        max = b;

    return max;
}

// N-14
Natural Natural::LCM_NN_N(const Natural &other) const {
    // связь НОД и НОК: (a * b) / GCD(a, b)
    Natural numerator = this->MUL_NN_N(other);   // числитель дроби
    Natural denominator = this->GCF_NN_N(other); // знаменатель дроби

    return numerator.DIV_NN_N(denominator); // деление числителя на знаменатель
}

std::ostream &operator<<(std::ostream &os, const Natural &number) {
    return os << number.as_string();
}