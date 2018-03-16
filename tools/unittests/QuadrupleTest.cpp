//===- QuadrupleTest.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/IR/Quadruple.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Testcases
//===----------------------------------------------------------------------===//
SKYPAT_F(QuadrupleTest, empty)
{
  Quadruple quadruple;
  ASSERT_TRUE(Quadruple::UnknownArch == quadruple.getArch());
  ASSERT_TRUE(Quadruple::UnknownArchVendor == quadruple.getArchVendor());
  ASSERT_TRUE(Quadruple::UnknownOS == quadruple.getOS());
  ASSERT_TRUE(Quadruple::UnknownEnvironment == quadruple.getEnvironment());
  ASSERT_TRUE(Quadruple::UnknownTool == quadruple.getTool());
  ASSERT_TRUE(Quadruple::UnknownToolVendor == quadruple.getToolVendor());
  ASSERT_TRUE(quadruple.getToolVersion().empty());
}

SKYPAT_F(QuadrupleTest, linaro_arm)
{
  Quadruple quadruple("arm-none-linux-gnueabihf-gcc-4.8.2-linaro");

  ASSERT_TRUE(Quadruple::arm == quadruple.getArch());
  ASSERT_TRUE(Quadruple::UnknownArchVendor == quadruple.getArchVendor());
  ASSERT_TRUE(Quadruple::Linux == quadruple.getOS());
  ASSERT_TRUE(Quadruple::GNUEABIHF == quadruple.getEnvironment());
  ASSERT_TRUE(Quadruple::DRIVER == quadruple.getTool());
  ASSERT_TRUE(Quadruple::Linaro == quadruple.getToolVendor());
  ASSERT_TRUE(0 == quadruple.getToolVersion().compare("4.8.2"));
}

SKYPAT_F(QuadrupleTest, sub_arch_test)
{
  Quadruple f1("armv8-none-linux-gnueabi-eglibc-2.17-ubuntu-odroid-ca8");

  ASSERT_TRUE(Quadruple::arm == f1.getArch());
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f1.getSubArch());

  Quadruple f2(f1);
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f2.getSubArch());

  Quadruple f3 = f1;
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f3.getSubArch());

  Quadruple f4("armv8", "apple", "linux");
  Quadruple f5("armv8", "none", "linux", "gnueabi");
  Quadruple f6("armv8", "none", "linux", "gnueabi", "gcc");
  Quadruple f7("armv8", "none", "linux", "gnueabi", "gcc", "4.8.2");
  Quadruple f8("armv8", "none", "linux", "gnueabi", "gcc", "4.8.2", "fsf");

  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f4.getSubArch());
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f5.getSubArch());
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f6.getSubArch());
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f7.getSubArch());
  EXPECT_TRUE(Quadruple::ARMSubArch_v8 == f8.getSubArch());

  EXPECT_TRUE(f1.getArchName().equals("armv8"));
  EXPECT_TRUE(f2.getArchName().equals("armv8"));
  EXPECT_TRUE(f3.getArchName().equals("armv8"));
  EXPECT_TRUE(f4.getArchName().equals("armv8"));
  EXPECT_TRUE(f5.getArchName().equals("armv8"));
  EXPECT_TRUE(f6.getArchName().equals("armv8"));
  EXPECT_TRUE(f7.getArchName().equals("armv8"));
  EXPECT_TRUE(f8.getArchName().equals("armv8"));
}

SKYPAT_F(QuadrupleTest, get_name_test)
{
  Quadruple q("arm-none-linux-gnueabi-g++-4.7.4-fsf-unknown-unknown");
  EXPECT_EQ(q.getArchName(), "arm");
  EXPECT_EQ(q.getOSName(), "linux");
  EXPECT_EQ(q.getEnvironmentName(), "gnueabi");
  EXPECT_EQ(q.getToolName(), "g++");
  EXPECT_EQ(q.getToolVendorName(), "fsf");
}

SKYPAT_F(QuadrupleTest, set_raw)
{
  Quadruple q("");
  ASSERT_NE(q.getArch(), Quadruple::arm);
  ASSERT_NE(q.getOS(), Quadruple::Linux);
  ASSERT_NE(q.getEnvironment(), Quadruple::GNUEABI);
  ASSERT_NE(q.getTool(), Quadruple::DRIVER);
  ASSERT_NE(q.getToolVendor(), Quadruple::FSF);

  q.setRaw("arm-none-linux-gnueabi-g++-4.7.4-fsf-unknown-unknown");
  EXPECT_EQ(q.getArch(), Quadruple::arm);
  EXPECT_EQ(q.getOS(), Quadruple::Linux);
  EXPECT_EQ(q.getEnvironment(), Quadruple::GNUEABI);
  EXPECT_EQ(q.getTool(), Quadruple::DRIVER);
  EXPECT_EQ(q.getToolVendor(), Quadruple::FSF);
}

SKYPAT_F(QuadrupleTest, canonicalize)
{
  Quadruple q1("arm-none-linux-gnueabihf-gcc-4.8.2-linaro");
  Quadruple q2;
  q2.setArch(Quadruple::arm);
  q2.setArchVendor(Quadruple::UnknownArchVendor);
  q2.setOS(Quadruple::Linux);
  q2.setEnvironment(Quadruple::GNUEABIHF);
  q2.setTool(Quadruple::DRIVER);
  q2.setToolVendor(Quadruple::Linaro);
  q2.setToolVersion("4.8.2");

  std::string c1;
  std::string c2;
  q1.canonical(c1);
  q2.canonical(c2);
  EXPECT_FALSE(0 == c1.compare(c2));

  Quadruple q3("arm-none-linux-gnueabihf-cpp-4.8.2-linaro");
  q2.setTool(Quadruple::CPP);

  std::string c3;
  q2.canonical(c2);
  q3.canonical(c3);
  EXPECT_TRUE(0 == c2.compare(c3));
}

SKYPAT_F(QuadrupleTest, sub_arch_test_2)
{
  Quadruple f1("sophonv1680-bitmain-linux-bmnet-all-0.1.0-none-tg");

  EXPECT_TRUE(Quadruple::sophon == f1.getArch());
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f1.getSubArch());

  Quadruple f2(f1);
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f2.getSubArch());

  Quadruple f3 = f1;
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f3.getSubArch());

  Quadruple f4("sophonv1680", "bitmain", "linux");
  Quadruple f5("sophonv1682", "bitmain", "linux", "bmnet");
  Quadruple f6("sophonv1680", "bitmain", "linux", "bmnet", "all");
  Quadruple f7("sophonv1682", "bitmain", "linux", "bmnet", "all", "4.8.2");
  Quadruple f8("sophonv1680", "bitmain", "linux", "bmnet", "all", "4.8.2", "none", "tg");

  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f4.getSubArch());
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1682 == f5.getSubArch());
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f6.getSubArch());
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1682 == f7.getSubArch());
  EXPECT_TRUE(Quadruple::SophonSubArch_vBM1680 == f8.getSubArch());
  EXPECT_TRUE(Quadruple::SophonArchCore_TG == f8.getArchCore());

  EXPECT_TRUE(f1.getArchName().equals("sophonv1680"));
  EXPECT_TRUE(f2.getArchName().equals("sophonv1680"));
  EXPECT_TRUE(f3.getArchName().equals("sophonv1680"));
  EXPECT_TRUE(f4.getArchName().equals("sophonv1680"));
  EXPECT_TRUE(f5.getArchName().equals("sophonv1682"));
  EXPECT_TRUE(f6.getArchName().equals("sophonv1680"));
  EXPECT_TRUE(f7.getArchName().equals("sophonv1682"));
  EXPECT_TRUE(f8.getArchName().equals("sophonv1680"));
}
