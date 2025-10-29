#include <iostream>

#include "natural.hpp"
#include "integer.hpp"
#include "rational.hpp"
#include "polynomial.hpp"

int main() {
    Natural a = Natural("893"); // a = 893
    Natural b = Natural("0000010"); // b = 10
    // Natural c = Natural("lol"); // ошибка

    std::cout << a << ' ' << b << '\n';
    std::cout<<b.COM_NN_D(a)<<'\n';

    Integer c = Integer("-1020");
    Integer d = Integer("439");

    std::cout << c << ' ' << d << '\n';
}
