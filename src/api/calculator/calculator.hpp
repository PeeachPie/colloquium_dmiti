#pragma once

#include "polynomial.hpp"
#include <string>
#include <stack>
#include <map>
#include <set>
#include <functional>

/*
 *  Класс Calculator — парсер и вычислитель выражений над полиномами от одной переменной x
 */
class Calculator {
public:
    Calculator() = default;

    /*
     * Упрощает строковое выражение, возвращая результат в виде строки
     * Например:
     *   "x^2 + 2x + 1"
     *   "DER(x^3 + x^2)"
     *   "GCD(x^2 - 1, x - 1)"
     */
    [[nodiscard]] std::string simplify_expression(const std::string& expression) const;

private:
    // --- внутренние методы ---

    [[nodiscard]] std::string insert_implicit_multiplication(const std::string& expr) const;

    // Преобразует инфиксную запись в постфиксную (обратную польскую нотацию)
    [[nodiscard]] std::vector<std::string> infix_to_postfix(const std::string& expr) const;

    // Выполняет вычисление выражения в постфиксной записи
    [[nodiscard]] Polynomial evaluate_postfix(const std::vector<std::string>& tokens) const;

    // Проверяет, является ли токен функцией (DER, GCD, FAC, NMR)
    [[nodiscard]] bool is_function(const std::string& token) const;

    // Проверяет, является ли токен оператором (+, -, *, /, %)
    [[nodiscard]] bool is_operator(const std::string& token) const;

    // Проверяет, является ли токен числом/коэффициентом
    [[nodiscard]] bool is_number(const std::string& token) const;

    // Проверяет, является ли токен мономом, например "x", "x^2", "3x^5"
    [[nodiscard]] bool is_monom(const std::string& token) const;

    // Возвращает приоритет оператора
    [[nodiscard]] int precedence(const std::string& op) const;

    // Преобразует строку (моном или число) в Polynomial
    [[nodiscard]] Polynomial parse_polynomial(const std::string& token) const;

    // Разбивает строку на токены
    [[nodiscard]] std::vector<std::string> tokenize(const std::string& expr) const;
};

