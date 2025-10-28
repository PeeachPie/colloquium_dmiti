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
    //std::cout<<b.COM_NN_D(a)<<'\n';
    //std::cout<<a.ADD_1N_N()<<'\n';
    //std::cout<<a.ADD_NN_N(b)<<'\n';
    //std::cout<<a.SUB_NN_N(b)<<'\n';
    //std::cout<< b.MUL_ND_N(5) <<'\n';
    //std::cout<< b.MUL_Nk_N(0) <<'\n';
    //std::cout<< b.SUB_NDN_N(a, 6) << '\n';
    //std::cout<< a.DIV_NN_Dk(b).first << '\n';
    //std::cout<< a.DIV_NN_Dk(b).second << '\n';
}
