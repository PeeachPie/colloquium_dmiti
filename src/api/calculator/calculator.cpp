#include "calculator.hpp"
#include "polynomial.hpp"
#include "rational.hpp"

#include <stdexcept>
#include <regex>
#include <string>
#include <cctype>
#include <iostream>

#define PARSER_DEBUG 0
#if PARSER_DEBUG
#define PARSER_LOG(msg) std::cerr << "[PARSER] " << msg << std::endl
#else
#define PARSER_LOG(msg)
#endif

// ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

std::string Calculator::normalize_spaces(const std::string& str) const {
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(),
        [](unsigned char c) { return std::isspace(c); }), result.end());
    return result;
}

Polynomial Calculator::simplify_expression(const std::string& expression) const {
    std::string normalized = normalize_spaces(expression);
    Polynomial result = parse_expression(normalized);
    return result.NORM_P_P();
}

size_t Calculator::find_matching_bracket(const std::string& str, size_t start_pos) const {
    if (start_pos >= str.length() || str[start_pos] != '(') {
        return std::string::npos;
    }
    
    int depth = 1;
    for (size_t i = start_pos + 1; i < str.length(); ++i) {
        if (str[i] == '(') {
            depth++;
        } else if (str[i] == ')') {
            depth--;
            if (depth == 0) {
                return i;
            }
        }
    }
    return std::string::npos;
}

bool Calculator::is_function_call(const std::string& str, size_t& func_name_end) const {
    func_name_end = 0;
    
    std::vector<std::string> func_names = {"GCD", "d/dx", "FAC", "NMR"};
    
    for (const auto& func : func_names) {
        if (str.length() >= func.length() && str.substr(0, func.length()) == func) {
            func_name_end = func.length();
            return true;
        }
    }
    
    return false;
}

std::pair<std::string, std::string> Calculator::parse_function_args(const std::string& str, size_t start_pos, size_t end_pos) const {
    std::string args_str = str.substr(start_pos, end_pos - start_pos);
    
    // Ищем запятую на верхнем уровне вложенности
    int depth = 0;
    size_t comma_pos = std::string::npos;
    int comma_count = 0;
    
    for (size_t i = 0; i < args_str.length(); ++i) {
        if (args_str[i] == '(') {
            depth++;
        } else if (args_str[i] == ')') {
            depth--;
        } else if (args_str[i] == ',' && depth == 0) {
            comma_count++;
            if (comma_count > 1) {
                throw std::invalid_argument("Функция GCD принимает не более двух параметров");
            }
            comma_pos = i;
        }
    }
    
    if (comma_pos != std::string::npos) {
        std::string first = args_str.substr(0, comma_pos);
        std::string second = args_str.substr(comma_pos + 1);
        return {first, second};
    }
    
    return {args_str, ""};
}

// ==================== ПАРСИНГ ПОЛИНОМА ====================

std::pair<int, std::string> Calculator::parse_term(const std::string& term) const {

    if (term.empty()) {
        return {0, "0"};
    }
    
    std::string coef_str = "1";
    std::string degree_str = "0";
    
    size_t x_pos = term.find('x');
    
    if (x_pos == std::string::npos) {
        coef_str = term;
        degree_str = "0";
    } else {
        std::string before_x = (x_pos > 0) ? term.substr(0, x_pos) : "";
        std::string after_x = (x_pos + 1 < term.length()) ? term.substr(x_pos + 1) : "";

        while (!before_x.empty() && (before_x.back() == '*' || before_x.back() == ' ')) {
            before_x.pop_back();
        }
        
        if (before_x.empty() || before_x == "+") {
            coef_str = "1";
        } else if (before_x == "-") {
            coef_str = "-1";
        } else {
            coef_str = before_x;
        }
        
        if (after_x.empty()) {
            degree_str = "1";
        } else if (!after_x.empty() && after_x[0] == '*') {
            if (after_x.length() > 1 && after_x[1] == '^') {
                size_t degree_start = 2;
                if (degree_start < after_x.length()) {
                    degree_str = after_x.substr(degree_start);
                } else {
                    degree_str = "1";
                }
            } else {
                degree_str = "1";
            }
        } else if (!after_x.empty() && after_x[0] == '^') {
            size_t degree_start = 1;
            if (degree_start < after_x.length()) {
                degree_str = after_x.substr(degree_start);
            } else {
                degree_str = "1";
            }
        } else {
            degree_str = "1";
        }
    }
    
    int degree = 0;
    try {
        degree = std::stoi(degree_str);
    } catch (...) {
        degree = 0;
    }
    
    return {degree, coef_str};
}

Polynomial Calculator::parse_polynomial(const std::string& str) const {
    if (str.empty()) {
        return Polynomial();
    }
    
    std::vector<std::pair<int, std::string>> terms_vector;
    
    std::vector<std::string> terms;
    std::string current_term;
    bool is_negative = false;
    bool first_term = true;
    
    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i];
        
        if (c == '+' || c == '-') {
            if (!current_term.empty()) {
                if (is_negative) {
                    current_term = "-" + current_term;
                }
                terms.push_back(current_term);
                current_term.clear();
                is_negative = false;
            } else if (first_term && c == '-') {
                is_negative = true;
            }
            if (c == '-') {
                is_negative = true;
            }
            first_term = false;
        } else {
            current_term += c;
            first_term = false;
        }
    }
    
    if (!current_term.empty()) {
        if (is_negative) {
            current_term = "-" + current_term;
        }
        terms.push_back(current_term);
    }

    if (terms.empty()) {
        terms.push_back(str);
    }
    
    for (const auto& term : terms) {
        if (term.empty()) continue;

        auto [degree, coef] = parse_term(term);
        if (!coef.empty() && coef != "0") {
            terms_vector.push_back({degree, coef});
        }
    }
    
    if (terms_vector.empty()) {
        return Polynomial();
    }
    
    return Polynomial(terms_vector);
}

// ==================== ПАРСИНГ ВЫРАЖЕНИЙ ====================

bool Calculator::is_simple_polynomial(const std::string& str) const {
    
    PARSER_LOG("is_simple_polynomial('" + str + "')");
    
    if (str.empty()) {
        PARSER_LOG("  -> false (empty)");
        return false;
    }

    if (str.find('(') != std::string::npos || str.find(')') != std::string::npos) {
        PARSER_LOG("  -> false (has brackets)");
        return false;
    }
    
    size_t func_name_end = 0;
    if (is_function_call(str, func_name_end)) {
        PARSER_LOG("  -> false (is function)");
        return false;
    }
    
    if (str.find('x') == std::string::npos) {
        PARSER_LOG("  -> true (no x, constant)");
        return true;
    }
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '*') {
            if (i + 1 < str.length() && (str[i+1] == 'x' || str[i+1] == '^')) {
                continue;
            }
            if (i + 1 < str.length() && std::isdigit(str[i+1])) {
                return false;
            }
        }
        
        if (str[i] == '/') {
            bool has_x_before = false;
            bool has_x_after = false;
            
            for (size_t j = 0; j < i; ++j) {
                if (str[j] == 'x') {
                    has_x_before = true;
                    break;
                }
            }
            
            for (size_t j = i + 1; j < str.length(); ++j) {
                if (str[j] == 'x') {
                    has_x_after = true;
                    break;
                }
            }
            
            if (has_x_before) {
                PARSER_LOG("  -> false (has x before /)");
                return false;
            }
        }
    }
    
    PARSER_LOG("  -> true (simple polynomial)");
    return true;
}

Polynomial Calculator::parse_expression(const std::string& str) const {
    PARSER_LOG("parse_expression('" + str + "')");
    
    if (str.empty()) {
        return Polynomial();
    }
    
    std::string trimmed = str;
    
    while (!trimmed.empty() && trimmed[0] == ' ') {
        trimmed = trimmed.substr(1);
    }
    while (!trimmed.empty() && trimmed.back() == ' ') {
        trimmed.pop_back();
    }
    
    if (trimmed.empty()) {
        return Polynomial();
    }
    
    size_t func_name_end_check = 0;
    bool is_gcd_function = false;
    if (is_function_call(trimmed, func_name_end_check)) {
        std::string func_name = trimmed.substr(0, func_name_end_check);
        if (func_name == "GCD") {
            is_gcd_function = true;
        }
    }
    
    if (!is_gcd_function) {
        int depth = 0;
        bool inside_function = false;
        
        for (size_t i = 0; i < trimmed.length(); ++i) {
            if (i + 3 <= trimmed.length() && trimmed.substr(i, 3) == "GCD") {
                inside_function = true;
            }
            
            if (trimmed[i] == '(') {
                depth++;
            } else if (trimmed[i] == ')') {
                depth--;
                if (depth == 0) {
                    inside_function = false;
                }
            } else if (trimmed[i] == ',' && !inside_function) {
                throw std::invalid_argument("Запятая разрешена только внутри функции GCD");
            }
        }
    }
    
    size_t func_name_end = 0;
    if (is_function_call(trimmed, func_name_end)) {
        std::string func_name = trimmed.substr(0, func_name_end);
        
        if (func_name == "GCD") {
            if (trimmed[func_name_end] != '(') {
                throw std::invalid_argument("Ожидается открывающая скобка после имени функции");
            }
            
            size_t args_start = func_name_end + 1;
            size_t args_end = find_matching_bracket(trimmed, func_name_end);
            
            if (args_end == std::string::npos) {
                throw std::invalid_argument("Не найдена закрывающая скобка");
            }
            
            auto [first_arg, second_arg] = parse_function_args(trimmed, args_start, args_end);
            
            if (second_arg.empty()) {
                throw std::invalid_argument("Функция GCD требует два аргумента");
            }
            
            Polynomial first = parse_expression(first_arg);
            Polynomial second = parse_expression(second_arg);
            
            if (!second.NZER_P_B()) {
                return first;
            }
            if (!first.NZER_P_B()) {
                return second;
            }
            
            if (first.DEG_P_N().as_string() == "0" && second.DEG_P_N().as_string() == "0") {
                Polynomial gcd_normalized = first.GCF_PP_P(second);
                
                auto [factor1, _] = first.FAC_P_Q();
                auto [factor2, __] = second.FAC_P_Q();
                
                Natural gcd_num = factor1.numerator().ABS_Z_N().GCF_NN_N(factor2.numerator().ABS_Z_N());
                Natural lcm_den = factor1.denominator().LCM_NN_N(factor2.denominator());
                Rational gcd_coeff(Integer(gcd_num.as_string()), lcm_den);
                
                return gcd_normalized.MUL_PQ_P(gcd_coeff);
            }
            
            return first.GCF_PP_P(second);
        } else if (func_name == "d/dx") {
            if (trimmed[func_name_end] != '(') {
                throw std::invalid_argument("Ожидается открывающая скобка после имени функции");
            }
            
            size_t args_start = func_name_end + 1;
            size_t args_end = find_matching_bracket(trimmed, func_name_end);
            
            if (args_end == std::string::npos) {
                throw std::invalid_argument("Не найдена закрывающая скобка");
            }
            
            std::string arg = trimmed.substr(args_start, args_end - args_start);
            Polynomial poly = parse_expression(arg);
            return poly.DER_P_P();
        } else if (func_name == "FAC") {
            if (trimmed[func_name_end] != '(') {
                throw std::invalid_argument("Ожидается открывающая скобка после имени функции");
            }
            
            size_t args_start = func_name_end + 1;
            size_t args_end = find_matching_bracket(trimmed, func_name_end);
            
            if (args_end == std::string::npos) {
                throw std::invalid_argument("Не найдена закрывающая скобка");
            }
            
            std::string arg = trimmed.substr(args_start, args_end - args_start);
            Polynomial poly = parse_expression(arg);
            auto [rational_part, polynomial_part] = poly.FAC_P_Q();
            return polynomial_part;
        } else if (func_name == "NMR") {
            if (trimmed[func_name_end] != '(') {
                throw std::invalid_argument("Ожидается открывающая скобка после имени функции");
            }
            
            size_t args_start = func_name_end + 1;
            size_t args_end = find_matching_bracket(trimmed, func_name_end);
            
            if (args_end == std::string::npos) {
                throw std::invalid_argument("Не найдена закрывающая скобка");
            }
            
            std::string arg = trimmed.substr(args_start, args_end - args_start);
            Polynomial poly = parse_expression(arg);
            return poly.NMR_P_P();
        }
    }
    
    if (is_simple_polynomial(trimmed)) {
        PARSER_LOG("  -> parsing as simple polynomial");
        return parse_polynomial(trimmed);
    }
    
    PARSER_LOG("  -> NOT simple polynomial, searching for operators");
    PARSER_LOG("  -> searching for low-priority operators (+, -)");
    int depth = 0;
    for (int i = static_cast<int>(trimmed.length()) - 1; i >= 0; --i) {
        if (trimmed[i] == ')') {
            depth++;
        } else if (trimmed[i] == '(') {
            depth--;
        } else if (depth == 0 && (trimmed[i] == '+' || trimmed[i] == '-')) {
            PARSER_LOG("     found '" + std::string(1, trimmed[i]) + "' at pos " + std::to_string(i));
            if (i == 0) continue;
            
            if (i > 0 && (trimmed[i-1] == '+' || trimmed[i-1] == '-' ||
                          trimmed[i-1] == '*' || trimmed[i-1] == '/' || trimmed[i-1] == '(')) {
                continue;
            }
            
            std::string left = trimmed.substr(0, i);
            std::string right = trimmed.substr(i + 1);
            
            Polynomial left_poly = parse_expression(left);
            Polynomial right_poly = parse_expression(right);
            
            PARSER_LOG("  -> found low-priority operator at pos " + std::to_string(i));
            if (trimmed[i] == '+') {
                return left_poly.ADD_PP_P(right_poly);
            } else {
                return left_poly.SUB_PP_P(right_poly);
            }
        }
    }
    
    PARSER_LOG("  -> searching for high-priority operators (*, /, %)");
    depth = 0;
    for (int i = static_cast<int>(trimmed.length()) - 1; i >= 0; --i) {
        if (trimmed[i] == ')') {
            depth++;
        } else if (trimmed[i] == '(') {
            depth--;
        } else if (depth == 0 && (trimmed[i] == '*' || trimmed[i] == '/' || trimmed[i] == '%')) {
            PARSER_LOG("     found '" + std::string(1, trimmed[i]) + "' at pos " + std::to_string(i));
            // Для * проверяем, что это не часть записи коэффициента (2*x или x*^2)
            if (trimmed[i] == '*' && i + 1 < trimmed.length() &&
                (trimmed[i+1] == 'x' || trimmed[i+1] == '^')) {
                continue;
            }
            if (trimmed[i] == '*' && i > 0 && 
                (trimmed[i-1] == 'x' || trimmed[i-1] == '^')) {
                continue;
            }
            
            // Если / найден на верхнем уровне - это всегда операция деления полиномов
            // Дробные коэффициенты (2/3x) парсятся в parse_polynomial -> parse_term
            
            std::string left = trimmed.substr(0, i);
            std::string right = trimmed.substr(i + 1);
            
            Polynomial left_poly = parse_expression(left);
            Polynomial right_poly = parse_expression(right);
            
            PARSER_LOG("  -> found high-priority operator '" + std::string(1, trimmed[i]) + "' at pos " + std::to_string(i));
            if (trimmed[i] == '*') {
                return left_poly.MUL_PP_P(right_poly);
            } else if (trimmed[i] == '/') {
                return left_poly.DIV_PP_P(right_poly);
            } else { // trimmed[i] == '%'
                return left_poly.MOD_PP_P(right_poly);
            }
        }
    }
    
    if (trimmed[0] == '(') {
        size_t closing = find_matching_bracket(trimmed, 0);
        
        if (closing == std::string::npos) {
            throw std::invalid_argument("Не найдена закрывающая скобка");
        }
        
        if (closing + 1 < trimmed.length()) {
            char next_char = trimmed[closing + 1];
            
            if (next_char == '(') {
                std::string first_part = trimmed.substr(1, closing - 1);
                size_t second_closing = find_matching_bracket(trimmed, closing + 1);
                
                if (second_closing == std::string::npos) {
                    throw std::invalid_argument("Не найдена закрывающая скобка для второго выражения");
                }
                
                std::string second_part = trimmed.substr(closing + 2, second_closing - closing - 2);
                
                Polynomial first = parse_expression(first_part);
                Polynomial second = parse_expression(second_part);
                
                if (second_closing + 1 < trimmed.length()) {
                    std::string remaining = trimmed.substr(second_closing + 1);
                    Polynomial product = first.MUL_PP_P(second);
                    Polynomial remaining_poly = parse_expression(remaining);
                    return product.MUL_PP_P(remaining_poly);
                }
                
                return first.MUL_PP_P(second);
            } else {
                std::string inner = trimmed.substr(1, closing - 1);
                std::string after = trimmed.substr(closing + 1);
                
                Polynomial inner_poly = parse_expression(inner);
                Polynomial after_poly = parse_expression(after);
                
                return inner_poly.MUL_PP_P(after_poly);
            }
        } else {
            std::string inner = trimmed.substr(1, closing - 1);
            return parse_expression(inner);
        }
    }
    
    return parse_polynomial(trimmed);
}
