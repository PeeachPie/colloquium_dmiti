#include "calculator.hpp"
#include <sstream>
#include <regex>
#include <stdexcept>
#include <cctype>
#include <algorithm>

std::string Calculator::insert_implicit_multiplication(const std::string& expr) const {
    std::string res;
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        res += c;
        
        if (i + 1 < expr.size()) {
            char next = expr[i + 1];
            
            // Случаи, когда нужно вставить умножение:
            // 1. число перед x: "3x" -> "3*x"
            // 2. x перед числом: "x3" -> "x*3" 
            // 3. число перед скобкой: "3(x+1)" -> "3*(x+1)"
            // 4. x перед скобкой: "x(x+1)" -> "x*(x+1)"
            // 5. скобка перед x или числом: "(x+1)(x+2)" -> "(x+1)*(x+2)"
            // 6. скобка перед скобкой: ")(" -> ")*("
            
            bool can_insert = false;
            
            if (isdigit(c) && (next == 'x' || next == 'X' || next == '(')) {
                can_insert = true; // число перед x или скобкой
            }
            else if ((c == 'x' || c == 'X') && (isdigit(next) || next == '(')) {
                can_insert = true; // x перед числом или скобкой
            }
            else if (c == ')' && (next == '(' || isdigit(next) || next == 'x' || next == 'X')) {
                can_insert = true; // скобка перед скобкой, числом или x
            }
            else if (isdigit(c) && next == '(') {
                can_insert = true; // число перед скобкой
            }
            
            if (can_insert && !(c == '/' && isdigit(next))) {
                res += '*';
            }
        }
    }
    return res;
}

std::vector<std::string> Calculator::tokenize(const std::string& expr) const {
    std::vector<std::string> tokens;
    std::string current;

    auto flush = [&]() {
        if (!current.empty()) {
            tokens.push_back(current);
            current.clear();
        }
    };

    for (char c : expr) {
        if (isspace(c)) continue;

        if (std::string("+-*/%(),").find(c) != std::string::npos) {
            flush();
            tokens.emplace_back(1, c);
        } else {
            current += c;
        }
    }
    flush();
    return tokens;
}

bool Calculator::is_operator(const std::string& token) const {
    static const std::set<std::string> ops = {"+", "-", "*", "/", "%"};
    return ops.count(token);
}

bool Calculator::is_function(const std::string& token) const {
    static const std::set<std::string> funcs = {"DER", "GCD", "FAC", "NMR"};
    return funcs.count(token);
}

bool Calculator::is_number(const std::string& token) const {
    // число или дробь, включая отрицательные
    return std::regex_match(token, std::regex("^-?[0-9]+(/[0-9]+)?$"));
}

bool Calculator::is_monom(const std::string& token) const {
    // Допустимые форматы: x, x^100, 3x, -2x^2, 1/2x^5, 5 (число без x)
    return std::regex_match(token, std::regex(R"(^(-?[0-9]+(/[0-9]+)?)?x(\^[0-9]+)?$)")) || is_number(token);
}

int Calculator::precedence(const std::string& op) const {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    return 0;
}

std::vector<std::string> Calculator::infix_to_postfix(const std::string& expr) const {
    std::string normalized = insert_implicit_multiplication(expr);
    std::vector<std::string> tokens = tokenize(normalized);
    std::vector<std::string> output;
    std::stack<std::string> ops;

    for (const std::string& token : tokens) {
        if (is_number(token) || is_monom(token) || token == "x") {
            output.push_back(token);
        } else if (is_function(token)) {
            ops.push(token);
        } else if (token == ",") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty() || ops.top() != "(") {
                throw std::runtime_error("Mismatched commas in expression");
            }
        } else if (is_operator(token)) {
            while (!ops.empty() && is_operator(ops.top()) &&
                   precedence(ops.top()) >= precedence(token)) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty()) {
                throw std::runtime_error("Mismatched parentheses");
            }
            ops.pop(); // remove "("
            if (!ops.empty() && is_function(ops.top())) {
                output.push_back(ops.top());
                ops.pop();
            }
        }
    }

    while (!ops.empty()) {
        if (ops.top() == "(") {
            throw std::runtime_error("Mismatched parentheses");
        }
        output.push_back(ops.top());
        ops.pop();
    }
    return output;
}

Polynomial Calculator::parse_polynomial(const std::string& token) const {
    // Обработка чистых чисел (константных полиномов)
    if (is_number(token)) {
        return Polynomial({{0, token}});
    }
    
    // Обработка мономов с x
    std::regex r(R"(^(-?[0-9]+(/[0-9]+)?)?x(\^([0-9]+))?$)");
    std::smatch m;

    if (!std::regex_match(token, m, r)) {
        throw std::runtime_error("Invalid polynomial token: " + token);
    }

    std::string coef_str = m[1].str();
    std::string pow_str = m[4].str();

    // Определяем коэффициент
    if (coef_str.empty()) {
        coef_str = "1"; // x означает 1*x
    } else if (coef_str == "-") {
        coef_str = "-1"; // -x означает -1*x
    }

    // Определяем степень
    int deg = pow_str.empty() ? 1 : stoi(pow_str);

    return Polynomial({{deg, coef_str}});
}

Polynomial Calculator::evaluate_postfix(const std::vector<std::string>& tokens) const {
    std::stack<Polynomial> st;

    for (const std::string& token : tokens) {
        if (is_number(token) || is_monom(token) || token == "x") {
            st.push(parse_polynomial(token));
        } else if (is_operator(token)) {
            if (st.size() < 2) throw std::runtime_error("Not enough operands for operator " + token);
            Polynomial b = st.top(); st.pop();
            Polynomial a = st.top(); st.pop();

            if (token == "+") st.push(a.ADD_PP_P(b));
            else if (token == "-") st.push(a.SUB_PP_P(b));
            else if (token == "*") st.push(a.MUL_PP_P(b));
            else if (token == "/") st.push(a.DIV_PP_P(b));
            else if (token == "%") st.push(a.MOD_PP_P(b));
        } else if (is_function(token)) {
            if (token == "DER" || token == "NMR") {
                if (st.empty()) throw std::runtime_error("Missing argument for function " + token);
                Polynomial p = st.top(); st.pop();
                if (token == "DER") st.push(p.DER_P_P());
                else if (token == "NMR") st.push(p.NMR_P_P());
            } else if (token == "FAC") {
                if (st.empty()) throw std::runtime_error("Missing argument for function FAC");
                Polynomial p = st.top(); st.pop();
                auto [r, q] = p.FAC_P_Q();
                st.push(q); // возвращаем полиномиальную часть
            } else if (token == "GCD") {
                if (st.size() < 2) throw std::runtime_error("Missing arguments for GCD");
                Polynomial b = st.top(); st.pop();
                Polynomial a = st.top(); st.pop();
                st.push(a.GCF_PP_P(b));
            }
        }
    }

    if (st.size() != 1) throw std::runtime_error("Invalid expression - stack has " + std::to_string(st.size()) + " elements");
    return st.top();
}

std::string Calculator::simplify_expression(const std::string& expression) const {
    try {
        std::string processed = expression;
        // Добавляем 0 перед унарным минусом в начале выражения
        if (!processed.empty() && processed[0] == '-') {
            processed = "0" + processed;
        }
        // Также обрабатываем унарные минусы после скобок
        processed = std::regex_replace(processed, std::regex("\\(-"), "(0-");
        
        auto postfix = infix_to_postfix(processed);
        Polynomial result = evaluate_postfix(postfix);
        return result.as_string();
    } catch (const std::exception& e) {
        return "Error: " + std::string(e.what());
    }
}