#include <iostream>

#include "natural.hpp"
#include "integer.hpp"
#include "rational.hpp"
#include "polynomial.hpp"

int main() {
    Natural a; // a = 0
    Natural b = Natural("000001234"); // b = 1234
    // Natural c = Natural("lol"); // ошибка

    std::cout << a << ' ' << b << '\n';
}