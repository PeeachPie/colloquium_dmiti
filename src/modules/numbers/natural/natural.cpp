#include "natural.hpp"

Natural::Natural(): n_(0), a_({'0'}) {};

Natural::Natural(std::string s) {
    int first_not_zero = 0;

    // находим первую не нулевую цифру, либо единственный ноль
    while (first_not_zero < s.size() && s[first_not_zero] == '0') {
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

    n_ = a_.size();
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

//N-3
Natural Natural::ADD_1N_N(){
    return ADD_NN_N(Natural("1"));
}

//N-4
Natural Natural::ADD_NN_N(const Natural& other) const {
    Natural smaller = *this; Natural larger = other;
    if(COM_NN_D(other) != 1) std::swap(smaller, larger);

    int transfer = 0;

    std::string result = larger.as_string();

    for(int i = 0; i < larger.n_; i++){  
        int dig_smaller = (i < smaller.n_) ? (smaller.a_[i] - '0') : 0;
        int dig_larger = larger.a_[i] - '0';
        int digit = dig_larger + dig_smaller + transfer;
        transfer = digit / 10;
        result[larger.n_ - i - 1] = (digit % 10) + '0';
    }

    if(transfer > 0){
        char ch_dig = transfer + '0';
        result = ch_dig + result;
    }

    return Natural(result);
}


Natural Natural::SUB_NN_N(const Natural& other) const{
    if(COM_NN_D(other) == 1){
        throw std::invalid_argument("Вычитаемое больше уменьшаемого");
    }
    
    if(COM_NN_D(other) == 0){
        return Natural();
    }

    std::string result = as_string();
    int borrow = 0;
    
    for(int i = 0; i < n_; i++){
        int dig1 =  (a_[i] - '0');
        int dig2 = (i < other.n_) ? (other.a_[i] - '0') : 0;
        
        dig1 -= borrow; 
        
        if(dig1 < dig2){
            dig1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        int diff = dig1 - dig2;
        result[n_ - i - 1] =  (diff + '0');
    }
    
    return Natural(result);
}



//N-6
Natural Natural::MUL_ND_N(int digit) const{
    if(digit > 10 || digit < 0) {
        throw std::invalid_argument("Множитель не может быть отрицательным и >= 9");
    }

    if(digit == 0 || (n_ == 0 && a_[0] == '0')) return Natural();
    if(digit == 1) return *this;

    std::string result;
    int transfer = 0;
    int i = 0;

    for(; i < n_; i++){
        int num = (a_[i] - '0') * digit + transfer;
        transfer = num / 10;
        char dig = (num % 10) + '0';
        result += dig;
    }

    if(transfer > 0){
        char dig = transfer + '0';
        result += dig;
    }

    reverse(result.begin(), result.end());

    return Natural(result);
}


//N-7
Natural Natural::MUL_Nk_N(int k) const{
    if(n_ == 0 && a_[0] == '0') return Natural();

    std::string result = as_string();

    for(int i = 0; i < k; i++){
        result += '0';
    }

    return Natural(result);
}


//N-9
Natural Natural::SUB_NDN_N(const Natural& other, int digit) const{
    Natural subtrahend = other.MUL_ND_N(digit);
    Natural result = SUB_NN_N(subtrahend);

    return result;
}


//N-10
std::pair<int, int> Natural::DIV_NN_Dk(const Natural& other) const{
    int k = 0; 
    while(COM_NN_D(other.MUL_Nk_N(k)) != 1){
        k++; //находим такое k, что другое, умноженное на 10^k превышает
             //наше число и берем предыдущее значение k
    }
    k--;

    Natural mul_k = other.MUL_Nk_N(k); 

    int dig = 1;
    for(; dig < 10; dig++){
        Natural tmp = mul_k.MUL_ND_N(dig); //идем по цифрам
        if(COM_NN_D(tmp) == 1) break;
    }
    dig--;

    return std::make_pair(dig, k);

}


std::ostream& operator << (std::ostream &os, const Natural &number) {
    return os << number.as_string();
}
