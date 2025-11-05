#pragma once

#include "polynomial.hpp"

#include <string>

class Calculator {
public:
    [[nodiscard]] Polynomial simplify_expression(const std::string& expression) const;

private:
    [[nodiscard]] Polynomial parse_polynomial(const std::string& str) const;
    
    [[nodiscard]] Polynomial parse_expression(const std::string& str) const;
    
    [[nodiscard]] size_t find_matching_bracket(const std::string& str, size_t start_pos) const;
    [[nodiscard]] std::string normalize_spaces(const std::string& str) const;
    [[nodiscard]] bool is_function_call(const std::string& str, size_t& func_name_end) const;
    [[nodiscard]] bool is_simple_polynomial(const std::string& str) const;
    
    [[nodiscard]] std::pair<std::string, std::string> parse_function_args(const std::string& str, size_t start_pos, size_t end_pos) const;
    
    [[nodiscard]] std::pair<int, std::string> parse_term(const std::string& term) const;
};

