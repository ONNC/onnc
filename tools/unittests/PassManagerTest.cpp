//===- PassManagerTest.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Core/Pass.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/Core/PassSupport.h>

using namespace skypat;
using namespace onnc;

namespace {

/** \class A
 */
class A : public ModulePass
{
public:
  static char ID;
  A() : ModulePass(ID) { }
  bool runOnModule(Module &pModule) { return true; }
};

char A::ID = 0;

/** \class B
 */
class B : public ModulePass
{
public:
  static char ID;

public:
  B() : ModulePass(ID), data(0x12) { }

  bool runOnModule(Module &pModule) { return true; }

  int data;
};

char B::ID = 0;

/** \class C
 */
class C : public ModulePass
{
public:
  static char ID;
  C() : ModulePass(ID) { }
  bool runOnModule(Module &pModule) { return true; }
};

char C::ID = 0;

} // anonymous namespace

INITIALIZE_PASS(A, "A")
INITIALIZE_PASS(B, "B")
INITIALIZE_PASS(C, "C")

//===----------------------------------------------------------------------===//
// PassManagerTest
//===----------------------------------------------------------------------===//
SKYPAT_F(PassManagerTest, add_passes)
{
  PassRegistry registry;
  EXPECT_EQ(registry.numOfPasses(), 0);
  EXPECT_TRUE(registry.isEmpty());

  InitializeAPass(registry);
  InitializeBPass(registry);
  InitializeCPass(registry);

  EXPECT_EQ(registry.numOfPasses(), 3);
  EXPECT_FALSE(registry.isEmpty());
}

SKYPAT_F(PassManagerTest, add_duplicated_passes)
{
  PassRegistry registry;
  EXPECT_EQ(registry.numOfPasses(), 0);
  EXPECT_TRUE(registry.isEmpty());

  InitializeAPass(registry);
  InitializeAPass(registry);
  InitializeAPass(registry);

  EXPECT_EQ(registry.numOfPasses(), 1);
  EXPECT_FALSE(registry.isEmpty());
}

SKYPAT_F(PassManagerTest, delete_passes)
{
  PassRegistry registry;
  InitializeAPass(registry);
  InitializeBPass(registry);
  InitializeCPass(registry);

  ASSERT_EQ(registry.numOfPasses(), 3);
  ASSERT_FALSE(registry.isEmpty());

  registry.clear();

  EXPECT_EQ(registry.numOfPasses(), 0);
  EXPECT_TRUE(registry.isEmpty());
}

SKYPAT_F(PassManagerTest, query_passes)
{
  PassRegistry registry;
  InitializeAPass(registry);
  InitializeBPass(registry);
  InitializeCPass(registry);

  ASSERT_EQ(registry.numOfPasses(), 3);
  ASSERT_FALSE(registry.isEmpty());

  const PassInfo* info = registry.getPassInfo(&B::ID);
  EXPECT_TRUE(info->getPassName().equals("B"));
  Pass* pass = info->createPass();
  EXPECT_EQ(((B*)pass)->data, 0x12);
  delete pass;
}
