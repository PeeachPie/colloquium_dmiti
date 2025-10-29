#include <gtest/gtest.h>
#include "natural.hpp"

TEST(NaturalTests, CreateDefault) {
    EXPECT_EQ(Natural().as_string(), "0");
}

TEST(NaturalTests, InvalidString) {
    EXPECT_THROW(Natural("number"), std::invalid_argument);
}

//N-1
TEST(NaturalTests, COM_NN_D_Naturals){
    Natural a("90"); Natural b("87"); Natural c("87");
    EXPECT_EQ(a.COM_NN_D(b), 2);
    EXPECT_EQ(b.COM_NN_D(a), 1);
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

//N-2
TEST(NaturalTests, NZER_N_B_Naturals){
    Natural a("287");
    Natural b("0");

    EXPECT_TRUE(a.NZER_N_B());
    EXPECT_FALSE(b.NZER_N_B());
}

//N-3
TEST(NaturalTests, ADD_1N_N_Naturals){
    Natural a("90"); Natural b("91");
    Natural c = a.ADD_1N_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

//N-4
TEST(NaturalTests, ADD_NN_N_Naturals){
    Natural a("10"); Natural b("15"); Natural c("25");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-5
TEST(NaturalTests, SUB_NN_N_Naturals){
    Natural a("380"); Natural b("5"); Natural c("375");
    Natural d = a.SUB_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-6
TEST(NaturalTests, MUL_ND_N_Naturals){
    Natural a("20"); int b = 5; Natural c("100");
    Natural d = a.MUL_ND_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-7
TEST(NaturalTests, MUL_Nk_N_Naturals){
    Natural a("2"); int k = 5; Natural c("200000");
    Natural d = a.MUL_Nk_N(k);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-8
TEST(NaturalTests, MUL_NN_N_Naturals){
    Natural a("123");
    Natural b("456");
    Natural c("56088");
    Natural d = a.MUL_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

//N-9
TEST(NaturalTests, SUB_NDN_N_Naturals){
    Natural a("200"); Natural b("4"); int dig = 5; Natural c("180");
    Natural d = a.SUB_NDN_N(b, dig);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-10
TEST(NaturalTests, DIV_NN_Dk_Naturals){
    Natural a("893"); Natural b("10"); int d = 8; int k = 1;
    std::pair<int, int> c = a.DIV_NN_Dk(b);
    EXPECT_EQ(c.first, 8);
    EXPECT_EQ(c.second, 1);
}

//N-11
TEST(NaturalTests, DIV_NN_N_Naturals){
    Natural a("138");
    Natural b("27");
    Natural c("5");
    Natural d = a.DIV_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

//N-12
TEST(NaturalTests, MOD_NN_N_Naturals){
    Natural a("329");
    Natural b("17");
    Natural c("6");
    Natural d = a.MOD_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

//N-13
TEST(NaturalTests, GCF_NN_N_Naturals){
    Natural a("1089");
    Natural b("345");
    Natural c("3");
    Natural d = a.GCF_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

//N-14
TEST(NaturalTests, LCM_NN_N_Naturals){
    Natural a("120");
    Natural b("298");
    Natural c("17880");
    Natural d = a.LCM_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}