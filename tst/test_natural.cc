#include <gtest/gtest.h>
#include "natural.hpp"

TEST(NaturalTests, CreateDefault) {
    EXPECT_EQ(Natural().as_string(), "0");
}

TEST(NaturalTests, InvalidString) {
    EXPECT_THROW(Natural("number"), std::invalid_argument);
}

TEST(NaturalTests, NZER_N_B_Naturals) {
    Natural a("287");
    Natural b("0");

    EXPECT_TRUE(a.NZER_N_B());
    EXPECT_FALSE(b.NZER_N_B());
}

TEST(NaturalTests, MUL_NN_N_Naturals) {
    Natural a("123");
    Natural b("456");
    Natural c("56088");
    Natural d = a.MUL_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, DIV_NN_N_Naturals) {
    Natural a("138");
    Natural b("27");
    Natural c("5");
    Natural d = a.DIV_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, MOD_NN_N_Naturals) {
    Natural a("329");
    Natural b("17");
    Natural c("6");
    Natural d = a.MOD_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, GCF_NN_N_Naturals) {
    Natural a("1089");
    Natural b("345");
    Natural c("3");
    Natural d = a.GCF_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}

TEST(NaturalTests, LCM_NN_N_Naturals) {
    Natural a("120");
    Natural b("298");
    Natural c("17880");
    Natural d = a.LCM_NN_N(b);

    EXPECT_EQ(c.COM_NN_D(d), 0);
}