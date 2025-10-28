#include <gtest/gtest.h>
#include "integer.hpp"

TEST(IntegerTests, CreateDefault) {
    EXPECT_EQ(Integer().as_string(), "0");
}

TEST(IntegerTests, InvalidString) {
    EXPECT_THROW(Integer("number"), std::invalid_argument);
}

TEST(IntegerTests, ABS_Z_N_Integers){
   Integer a("-20"); Natural b("20");
   EXPECT_EQ(a.ABS_Z_N(), b);
}

TEST(IntegerTests, SGN_Z_D_Integers){
   Integer a("-20"); Integer b("20"); Integer c();
   EXPECT_EQ(a.SGN_Z_D(), -1);
   EXPECT_EQ(b.SGN_Z_D(), 1);
   EXPECT_EQ(c.SGN_Z_D(), 0);
}

TEST(IntegerTests, MUL_ZM_Z_Integers){
   Integer a("-20"); Integer b("20");
   EXPECT_EQ(a.MUL_ZM_Z(), b);
}

TEST(IntegerTests, TRANS_N_Z_Integers){
   Natural a("20"); Integer b("20");
   Integer c(); c.TRANS_N_Z(a);
   EXPECT_EQ(c, b);
}

TEST(IntegerTests, TRANS_Z_N_Integers){
   Integer a("20"); Natural b("20");
   Natural c = a.TRANS_Z_N();
   EXPECT_EQ(c, b);
}

TEST(IntegerTests, ADD_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15"); Integer c("5");
   Integer d = a.ADD_ZZ_Z(b);
   EXPECT_EQ(d, c);
}

TEST(IntegerTests, SUB_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15"); Integer c("35");
   Integer d = a.SUB_ZZ_Z(b);
   EXPECT_EQ(d, c);
}

TEST(IntegerTests, MUL_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15"); Integer c("-300");
   Integer d = a.MUL_ZZ_Z(b);
   EXPECT_EQ(d, c);
}
