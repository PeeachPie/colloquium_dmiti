#include <gtest/gtest.h>
#include "natural.hpp"

TEST(NaturalTests, CreateDefault) {
    EXPECT_EQ(Natural().as_string(), "0");
}

TEST(NaturalTests, InvalidString) {
    EXPECT_THROW(Natural("number"), std::invalid_argument);
}

TEST(NaturalTests, COM_NN_D_Naturals){
    Natural a("90"); Natural b("87"); Natural c("87");
    EXPECT_EQ(a.COM_NN_D(b), 2);
    EXPECT_EQ(b.COM_NN_D(a), 1);
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(NaturalTests, ADD_1N_N_Naturals){
    Natural a("90"); Natural b("91");
    Natural c = a.ADD_1N_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(NaturalTests, ADD_NN_N_Naturals){
    Natural a("10"); Natural b("15"); Natural c("25");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, SUB_NN_N_Naturals){
    Natural a("380"); Natural b("5"); Natural c("375");
    Natural d = a.SUB_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, MUL_ND_N_Naturals){
    Natural a("20"); int b = 5; Natural c("100");
    Natural d = a.MUL_ND_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, MUL_Nk_N_Naturals){
    Natural a("2"); int k = 5; Natural c("200000");
    Natural d = a.MUL_Nk_N(k);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, SUB_NDN_N_Naturals){
    Natural a("200"); Natural b("4"); int dig = 5; Natural c("180");
    Natural d = a.SUB_NDN_N(b, dig);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, DIV_NN_Dk_Naturals){
    Natural a("893"); Natural b("10"); int d = 8; int k = 1;
    std::pair<int, int> c = a.DIV_NN_Dk(b);
    EXPECT_EQ(c.first, 8);
    EXPECT_EQ(c.second, 1);
}
