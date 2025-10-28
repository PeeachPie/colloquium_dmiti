#include "integer.hpp"
#include <stdexcept>
#include <algorithm>

namespace {

std::string strip_leading_zeros(const std::string& s) {
    size_t i = 0;
    while (i + 1 < s.size() && s[i] == '0') i++;
    return s.substr(i);
}

int compare_abs_str(const std::string& a, const std::string& b) {
    std::string aa = strip_leading_zeros(a);
    std::string bb = strip_leading_zeros(b);
    if (aa.size() != bb.size()) return aa.size() > bb.size() ? 1 : -1;
    if (aa == bb) return 0;
    return aa > bb ? 1 : -1;
}

std::string add_abs_str(const std::string& a, const std::string& b) {
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    int carry = 0;
    std::string out;
    while (i >= 0 || j >= 0 || carry) {
        int da = (i >= 0 ? a[i] - '0' : 0);
        int db = (j >= 0 ? b[j] - '0' : 0);
        int s = da + db + carry;
        out.push_back(char('0' + (s % 10)));
        carry = s / 10;
        i--; j--;
    }
    std::reverse(out.begin(), out.end());
    return strip_leading_zeros(out);
}

std::string sub_abs_str_geq(const std::string& a, const std::string& b) {
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    int borrow = 0;
    std::string out;
    while (i >= 0) {
        int da = a[i] - '0' - borrow;
        int db = (j >= 0 ? b[j] - '0' : 0);
        if (da < db) {
            da += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        int d = da - db;
        out.push_back(char('0' + d));
        i--; j--;
    }
    while (out.size() > 1 && out.back() == '0') out.pop_back();
    std::reverse(out.begin(), out.end());
    return strip_leading_zeros(out);
}

std::string mul_abs_str(const std::string& a, const std::string& b) {
    std::string aa = strip_leading_zeros(a);
    std::string bb = strip_leading_zeros(b);
    if (aa == "0" || bb == "0") return "0";
    std::vector<int> res(aa.size() + bb.size(), 0);
    for (int i = (int)aa.size() - 1; i >= 0; --i) {
        for (int j = (int)bb.size() - 1; j >= 0; --j) {
            int p = (aa[i] - '0') * (bb[j] - '0');
            int k = i + j + 1;
            int sum = res[k] + p;
            res[k] = sum % 10;
            res[k - 1] += sum / 10;
        }
    }
    std::string out;
    bool leading = true;
    for (int v : res) {
        if (leading && v == 0) continue;
        leading = false;
        out.push_back(char('0' + v));
    }
    if (out.empty()) out = "0";
    return out;
}

std::pair<std::string, std::string> divmod_abs_str(const std::string& a, const std::string& b) {
    std::string aa = strip_leading_zeros(a);
    std::string bb = strip_leading_zeros(b);
    if (bb == "0") throw std::invalid_argument("Деление на ноль");
    if (compare_abs_str(aa, bb) < 0) return {"0", aa};
    std::string q;
    std::string r = "0";
    for (char ch : aa) {
        if (r == "0") r = std::string(1, ch);
        else r.push_back(ch);
        int digit = 0;
        int lo = 0, hi = 9;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            std::string prod = mul_abs_str(bb, std::string(1, char('0' + mid)));
            if (compare_abs_str(prod, r) <= 0) { digit = mid; lo = mid + 1; }
            else { hi = mid - 1; }
        }
        q.push_back(char('0' + digit));
        if (digit > 0) {
            std::string sub = mul_abs_str(bb, std::string(1, char('0' + digit)));
            r = sub_abs_str_geq(r, sub);
        }
        r = strip_leading_zeros(r);
    }
    q = strip_leading_zeros(q);
    r = strip_leading_zeros(r);
    return {q, r};
}

}

Integer::Integer() : Natural(), s_(false) {}

Integer::Integer(std::string number) : s_(false), Natural("") {
    if (number.empty()) {
        throw std::invalid_argument("Строка должна содержать число");
    }

    bool negative = false;
    if (number[0] == '+' || number[0] == '-') {
        negative = (number[0] == '-');
        number.erase(number.begin());
    }

    Natural tmp(number);

    this->a_ = tmp.as_string();
    std::reverse(this->a_.begin(), this->a_.end());
    this->n_ = static_cast<int>(this->a_.size());

    s_ = (tmp.as_string() != "0") && negative;
}

std::string Integer::as_string() const {
    std::string abs_str = Natural::as_string();
    if (abs_str == "0") {
        return abs_str;
    }
    return (s_ ? std::string("-") + abs_str : abs_str);
}

Natural Integer::ABS_Z_N() const {
    return Natural(Natural::as_string());
}

int Integer::POZ_Z_D() const {
    std::string abs_str = Natural::as_string();
    if (abs_str == "0") return 0;
    return s_ ? 1 : 2;
}

Integer& Integer::MUL_ZM_Z() {
    if (Natural::as_string() != "0") {
        s_ = !s_;
    }
    return *this;
}

Integer Integer::TRANS_N_Z(const Natural& natural) {
    return Integer(natural.as_string());
}

Natural Integer::TRANS_Z_N() const {
    if (POZ_Z_D() == 1) {
        throw std::invalid_argument("Отрицательное целое нельзя преобразовать в натуральное");
    }
    return Natural(Natural::as_string());
}

Integer Integer::ADD_ZZ_Z(const Integer& other) const {
    const std::string a = this->Natural::as_string();
    const std::string b = other.Natural::as_string();
    bool sa = this->s_;
    bool sb = other.s_;

    if (a == "0") {
        return Integer((sb ? std::string("-") : std::string("")) + b);
    }
    if (b == "0") {
        return Integer((sa ? std::string("-") : std::string("")) + a);
    }

    std::string res_abs;
    bool res_neg = false;
    if (sa == sb) {
        res_abs = add_abs_str(a, b);
        res_neg = sa;
    } else {
        int cmp = compare_abs_str(a, b);
        if (cmp == 0) {
            return Integer("0");
        } else if (cmp > 0) {
            res_abs = sub_abs_str_geq(a, b);
            res_neg = sa;
        } else {
            res_abs = sub_abs_str_geq(b, a);
            res_neg = sb;
        }
    }
    if (res_abs == "0") return Integer("0");
    return Integer((res_neg ? std::string("-") : std::string("")) + res_abs);
}

Integer Integer::SUB_ZZ_Z(const Integer& other) const {
    Integer neg_other = Integer(other.as_string());
    if (neg_other.Natural::as_string() != "0") {
        neg_other.s_ = !other.s_;
    }
    return this->ADD_ZZ_Z(neg_other);
}

Integer Integer::MUL_ZZ_Z(const Integer& other) const {
    const std::string a = this->Natural::as_string();
    const std::string b = other.Natural::as_string();
    if (a == "0" || b == "0") return Integer("0");
    std::string prod = mul_abs_str(a, b);
    bool neg = (this->s_ != other.s_);
    return Integer((neg ? std::string("-") : std::string("")) + prod);
}

Integer Integer::DIV_ZZ_Z(const Integer& other) const {
    const std::string a = this->Natural::as_string();
    const std::string b = other.Natural::as_string();
    if (b == "0") throw std::invalid_argument("Деление на ноль");
    if (a == "0") return Integer("0");
    auto [q0, r0] = divmod_abs_str(a, b);
    bool signs_diff = (this->s_ != other.s_);
    if (!signs_diff) {
        return Integer((this->s_ ? std::string("-") : std::string("")) + q0);
    } else {
        if (r0 == "0") {
            return Integer(std::string("-") + q0);
        }
        std::string q_inc = add_abs_str(q0, "1");
        return Integer(std::string("-") + q_inc);
    }
}

Integer Integer::MOD_ZZ_Z(const Integer& other) const {
    const std::string a = this->Natural::as_string();
    const std::string b = other.Natural::as_string();
    if (b == "0") throw std::invalid_argument("Деление на ноль");
    if (a == "0") return Integer("0");
    auto [q0, r0] = divmod_abs_str(a, b);
    bool signs_diff = (this->s_ != other.s_);
    if (!signs_diff) {
        return Integer(r0);
    } else {
        if (r0 == "0") return Integer("0");
        std::string bb = strip_leading_zeros(b);
        std::string rr = sub_abs_str_geq(bb, r0);
        return Integer(rr);
    }
}
