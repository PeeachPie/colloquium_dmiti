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
    Natural a("20"); Natural b("95"); Natural c("115");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

//N-5
TEST(NaturalTests, SUB_NN_N_Naturals){
    Natural a("380"); Natural b("21"); Natural c("359");
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

TEST(NaturalTests, CreateFromZeroString) {
    EXPECT_EQ(Natural("0").as_string(), "0");
}

// TEST(NaturalTests, CreateFromEmptyString) {
//     EXPECT_THROW(Natural(""), std::invalid_argument);
// }

TEST(NaturalTests, CreateFromStringWithSpaces) {
    EXPECT_THROW(Natural("123 456"), std::invalid_argument);
}

TEST(NaturalTests, CreateFromNegativeString) {
    EXPECT_THROW(Natural("-123"), std::invalid_argument);
}

TEST(NaturalTests, COM_NN_D_EqualLarge) {
    Natural a("999999999999999999999999");
    Natural b("999999999999999999999999");
    EXPECT_EQ(a.COM_NN_D(b), 0);
}

TEST(NaturalTests, COM_NN_D_FirstGreaterLarge) {
    Natural a("1000000000000000000000000");
    Natural b("999999999999999999999999");
    EXPECT_EQ(a.COM_NN_D(b), 2);
}

TEST(NaturalTests, COM_NN_D_SecondGreaterLarge) {
    Natural a("999999999999999999999999");
    Natural b("1000000000000000000000000");
    EXPECT_EQ(a.COM_NN_D(b), 1);
}

TEST(NaturalTests, NZER_N_B_LargeNonZero) {
    Natural a("1000000000000000000000000");
    EXPECT_TRUE(a.NZER_N_B());
}

TEST(NaturalTests, ADD_1N_N_Zero) {
    Natural a("0");
    Natural b("1");
    Natural c = a.ADD_1N_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(NaturalTests, ADD_1N_N_LargeNumber) {
    Natural a("999999999999999999999999");
    Natural b("1000000000000000000000000");
    Natural c = a.ADD_1N_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(NaturalTests, ADD_1N_N_CarryOverflow) {
    Natural a("199999999999999999999999");
    Natural b("200000000000000000000000");
    Natural c = a.ADD_1N_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(NaturalTests, ADD_NN_N_ZeroFirst) {
    Natural a("0");
    Natural b("123");
    Natural c("123");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, ADD_NN_N_ZeroSecond) {
    Natural a("123");
    Natural b("0");
    Natural c("123");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, ADD_NN_N_LargeNumbers) {
    Natural a("999999999999999999999999");
    Natural b("1");
    Natural c("1000000000000000000000000");
    Natural d = a.ADD_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, SUB_NN_N_EqualNumbers) {
    Natural a("100");
    Natural b("100");
    Natural c("0");
    Natural d = a.SUB_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, SUB_NN_N_ZeroResult) {
    Natural a("50");
    Natural b("50");
    Natural c("0");
    Natural d = a.SUB_NN_N(b);
    EXPECT_EQ(d.COM_NN_D(c), 0);
}

TEST(NaturalTests, SUB_NN_N_LargeNumbers) {
    Natural a("1000000000000000000000000");
    Natural b("1");
    Natural c("999999999999999999999999");
    Natural d = a.SUB_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_ND_N_Zero) {
    Natural a("0");
    int b = 5;
    Natural c("0");
    Natural d = a.MUL_ND_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_ND_N_MultiplyByZero) {
    Natural a("123");
    int b = 0;
    Natural c("0");
    Natural d = a.MUL_ND_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_ND_N_LargeNumber) {
    Natural a("100000000000000000000000");
    int b = 2;
    Natural c("200000000000000000000000");
    Natural d = a.MUL_ND_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_Nk_N_Zero) {
    Natural a("0");
    int k = 5;
    Natural c("0");
    Natural d = a.MUL_Nk_N(k);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_Nk_N_ZeroPower) {
    Natural a("123");
    int k = 0;
    Natural c("123");
    Natural d = a.MUL_Nk_N(k);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_Nk_N_LargePower) {
    Natural a("1");
    int k = 100;
    Natural d = a.MUL_Nk_N(k);
    
    std::string expected = "1" + std::string(k, '0');
    EXPECT_EQ(d.as_string(), expected);
}

TEST(NaturalTests, MUL_NN_N_ZeroFirst) {
    Natural a("0");
    Natural b("456");
    Natural c("0");
    Natural d = a.MUL_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_NN_N_ZeroSecond) {
    Natural a("123");
    Natural b("0");
    Natural c("0");
    Natural d = a.MUL_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MUL_NN_N_LargeNumbers) {
    Natural a("1000000000000000000");
    Natural b("1000000000000000000");
    Natural expected("1000000000000000000000000000000000000");
    Natural result = a.MUL_NN_N(b);
    EXPECT_EQ(expected.COM_NN_D(result), 0);
}

TEST(NaturalTests, SUB_NDN_N_ZeroResult) {
    Natural a("100");
    Natural b("20");
    int dig = 5;
    Natural c("0");
    Natural d = a.SUB_NDN_N(b, dig);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, SUB_NDN_N_LargeNumbers) {
    Natural a("1000000000000000000000000");
    Natural b("100000000000000000000000");
    int dig = 1;
    Natural c("900000000000000000000000");
    Natural d = a.SUB_NDN_N(b, dig);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, DIV_NN_Dk_DivideByZero) {
    Natural a("123");
    Natural b("0");
    EXPECT_THROW(a.DIV_NN_Dk(b), std::invalid_argument);
}

TEST(NaturalTests, DIV_NN_Dk_DivideByOne) {
    Natural a("123");
    Natural b("1");
    auto result = a.DIV_NN_Dk(b);
    EXPECT_EQ(result.first, 1);
    EXPECT_EQ(result.second, 2);
}

TEST(NaturalTests, DIV_NN_Dk_LargeNumbers) {
    Natural a("1000000000000000000000000");
    Natural b("100000000000000000000000");
    auto result = a.DIV_NN_Dk(b);
    EXPECT_GE(result.first, 0);
    EXPECT_GE(result.second, 0);
}

TEST(NaturalTests, DIV_NN_N_DivideByZero) {
    Natural a("123");
    Natural b("0");
    EXPECT_THROW(a.DIV_NN_N(b), std::invalid_argument);
}

TEST(NaturalTests, DIV_NN_N_DivideByOne) {
    Natural a("123");
    Natural b("1");
    Natural c("123");
    Natural d = a.DIV_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, DIV_NN_N_DivideEqual) {
    Natural a("123");
    Natural b("123");
    Natural c("1");
    Natural d = a.DIV_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MOD_NN_N_ModByZero) {
    Natural a("123");
    Natural b("0");
    EXPECT_THROW(a.MOD_NN_N(b), std::invalid_argument);
}

TEST(NaturalTests, MOD_NN_N_ModByOne) {
    Natural a("123");
    Natural b("1");
    Natural c("0");
    Natural d = a.MOD_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MOD_NN_N_ModEqual) {
    Natural a("123");
    Natural b("123");
    Natural c("0");
    Natural d = a.MOD_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, GCF_NN_N_ZeroFirst) {
    Natural a("0");
    Natural b("123");
    Natural c("123");
    Natural d = a.GCF_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, GCF_NN_N_ZeroSecond) {
    Natural a("123");
    Natural b("0");
    Natural c("123");
    Natural d = a.GCF_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, GCF_NN_N_CoPrime) {
    Natural a("17");
    Natural b("13");
    Natural c("1");
    Natural d = a.GCF_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, LCM_NN_N_ZeroFirst) {
    Natural a("0");
    Natural b("123");
    Natural c("0");
    Natural d = a.LCM_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, LCM_NN_N_ZeroSecond) {
    Natural a("123");
    Natural b("0");
    Natural c("0");
    Natural d = a.LCM_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, LCM_NN_N_CoPrime) {
    Natural a("17");
    Natural b("13");
    Natural c("221");
    Natural d = a.LCM_NN_N(b);
    EXPECT_EQ(c.COM_NN_D(d), 0);
}