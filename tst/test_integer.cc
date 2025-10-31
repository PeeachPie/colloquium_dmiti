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
    Natural c = a.ABS_Z_N();
    EXPECT_EQ(b.COM_NN_D(c), 0);
}

TEST(IntegerTests, SGN_Z_D_Integers){
   Integer a("-20"); Integer b("20"); Integer c;
   EXPECT_EQ(a.SGN_Z_D(), -1);
   EXPECT_EQ(b.SGN_Z_D(), 1);
   EXPECT_EQ(c.SGN_Z_D(), 0);
}

TEST(IntegerTests, COM_ZZ_D_Integers){
   Integer a("-198"); Integer b("115"); Integer c;
   EXPECT_EQ(a.COM_ZZ_D(a), 0);
   EXPECT_EQ(a.COM_ZZ_D(b), -1);
   EXPECT_EQ(a.COM_ZZ_D(c), -1);
   EXPECT_EQ(b.COM_ZZ_D(b), 0);
   EXPECT_EQ(b.COM_ZZ_D(a), 1);
   EXPECT_EQ(b.COM_ZZ_D(c), 1);
   EXPECT_EQ(c.COM_ZZ_D(c), 0);
   EXPECT_EQ(c.COM_ZZ_D(a), 1);
   EXPECT_EQ(c.COM_ZZ_D(b), -1);
}

TEST(IntegerTests, MUL_ZM_Z_Integers){
   Integer a("-20"); Integer b("20");
   Integer c = a.MUL_ZM_Z();
   Integer d = b.MUL_ZM_Z();
   EXPECT_EQ(c.SGN_Z_D(), 1);
   EXPECT_EQ(d.SGN_Z_D(), -1);
}

TEST(IntegerTests, TRANS_N_Z_Integers){
   Natural a("20"); Integer b("20");
   Integer c = c.TRANS_N_Z(a);
   EXPECT_EQ(c.COM_ZZ_D(b), 0);
}

TEST(IntegerTests, TRANS_Z_N_Integers){
   Integer a("20"); Natural b("20");
   Natural c = a.TRANS_Z_N();
   EXPECT_EQ(c.COM_NN_D(b), 0);
}

TEST(IntegerTests, ADD_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15");
   Integer c("-20"); Integer d("15");
   Integer asb = a.ADD_ZZ_Z(b);
   Integer asd = a.ADD_ZZ_Z(d);
   Integer csb = c.ADD_ZZ_Z(b);
   Integer csd = c.ADD_ZZ_Z(d);
   Integer r1("5"); Integer r2("35");
   Integer r3("-35"); Integer r4("-5");
   EXPECT_EQ(r1.COM_ZZ_D(asb), 0);
   EXPECT_EQ(r2.COM_ZZ_D(asd), 0);
   EXPECT_EQ(r3.COM_ZZ_D(csb), 0);
   EXPECT_EQ(r4.COM_ZZ_D(csd), 0);
}

TEST(IntegerTests, SUB_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15");
   Integer c("-20"); Integer d("15");
   Integer asb = a.SUB_ZZ_Z(b);
   Integer asd = a.SUB_ZZ_Z(d);
   Integer csb = c.SUB_ZZ_Z(b);
   Integer csd = c.SUB_ZZ_Z(d);
   Integer r1("35"); Integer r2("5");
   Integer r3("-5"); Integer r4("-35");
   EXPECT_EQ(r1.COM_ZZ_D(asb), 0);
   EXPECT_EQ(r2.COM_ZZ_D(asd), 0);
   EXPECT_EQ(r3.COM_ZZ_D(csb), 0);
   EXPECT_EQ(r4.COM_ZZ_D(csd), 0);
}

TEST(IntegerTests, SUB_ZZ_Z_Integers_Lim){
   Integer a("20"); Integer b("-20");
   Integer asb = a.SUB_ZZ_Z(b);
   Integer asa = a.SUB_ZZ_Z(a);
   Integer bsb = b.SUB_ZZ_Z(b);
   Integer bsa = b.SUB_ZZ_Z(a);
   Integer r1("40"); Integer r2;
   Integer r3("-40");
   EXPECT_EQ(r1.COM_ZZ_D(asb), 0);
   EXPECT_EQ(r2.COM_ZZ_D(asa), 0);
   EXPECT_EQ(r2.COM_ZZ_D(bsb), 0);
   EXPECT_EQ(r3.COM_ZZ_D(bsa), 0);
}

TEST(IntegerTests, MUL_ZZ_Z_Integers){
   Integer a("20"); Integer b("-15"); Integer c("-300");
   Integer d = a.MUL_ZZ_Z(b);
   EXPECT_EQ(d.COM_ZZ_D(c), 0);
}

TEST(IntegerTests, MUL_ZZ_Z_Integers_Zero){
    Integer a("89"); Integer b;
    Integer c = a.MUL_ZZ_Z(b);
    EXPECT_EQ(c.COM_ZZ_D(b), 0);
}

TEST(IntegerTests, DIV_ZZ_Z_Integers){
    Integer a("20"); Integer b("-15");
    Integer c("-20"); Integer d("15");
    Integer atb = a.DIV_ZZ_Z(b);
    Integer atd = a.DIV_ZZ_Z(d);
    Integer ctb = c.DIV_ZZ_Z(b);
    Integer ctd = c.DIV_ZZ_Z(d);
    Integer r1("-2"); Integer r2("1");
    Integer r3("1"); Integer r4("-2");
    EXPECT_EQ(r1.COM_ZZ_D(atb), 0);
    EXPECT_EQ(r2.COM_ZZ_D(atd), 0);
    EXPECT_EQ(r3.COM_ZZ_D(ctb), 0);
    EXPECT_EQ(r4.COM_ZZ_D(ctd), 0);
}

TEST(IntegerTests, DIV_ZZ_Z_Integers_ZeroMod){
    Integer a("-48"); Integer b("6");
    Integer c("48"); Integer d("-6");
    Integer atb = a.DIV_ZZ_Z(b);
    Integer atd = a.DIV_ZZ_Z(d);
    Integer ctb = c.DIV_ZZ_Z(b);
    Integer ctd = c.DIV_ZZ_Z(d);
    Integer r1("-8"); Integer r2("8");
    EXPECT_EQ(r1.COM_ZZ_D(atb), 0);
    EXPECT_EQ(r2.COM_ZZ_D(atd), 0);
    EXPECT_EQ(r2.COM_ZZ_D(ctb), 0);
    EXPECT_EQ(r1.COM_ZZ_D(ctd), 0);
}

TEST(IntegerTests, DIV_ZZ_Z_Integers_Zero){
    Integer a("20"); Integer b;
    EXPECT_THROW(a.DIV_ZZ_Z(b), std::invalid_argument);
    Integer c = b.DIV_ZZ_Z(a);
    EXPECT_EQ(c.COM_ZZ_D(b), 0);
}

TEST(IntegerTests, MOD_ZZ_Z_Integers){
    Integer a("20"); Integer b("-15");
    Integer c("-20"); Integer d("15");
    Integer atb = a.MOD_ZZ_Z(b);
    Integer atd = a.MOD_ZZ_Z(d);
    Integer ctb = c.MOD_ZZ_Z(b);
    Integer ctd = c.MOD_ZZ_Z(d);
    Integer r1("10"); Integer r2("5");
    Integer r3("5"); Integer r4("10");
    EXPECT_EQ(r1.COM_ZZ_D(atb), 0);
    EXPECT_EQ(r2.COM_ZZ_D(atd), 0);
    EXPECT_EQ(r3.COM_ZZ_D(ctb), 0);
    EXPECT_EQ(r4.COM_ZZ_D(ctd), 0);
}

TEST(IntegerTests, MOD_ZZ_Z_Integers_Zero){
    Integer a("20"); Integer b;
    EXPECT_THROW(a.MOD_ZZ_Z(b), std::invalid_argument);
    Integer c = b.MOD_ZZ_Z(a);
    EXPECT_EQ(c.COM_ZZ_D(b), 0);
}
