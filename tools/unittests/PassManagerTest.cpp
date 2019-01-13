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
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassManager.h>
#include <onnc/IR/Module.h>

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

  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  StringRef getPassName() const { return "A"; }
};

char A::ID = 0;

/** \class B
 */
class B : public ModulePass
{
public:
  static char ID;

public:
  B() : ModulePass(ID) { }

  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const {
    pUsage.addRequiredID(A::ID);
  }

  StringRef getPassName() const { return "B"; }
};

char B::ID = 0;

/** \class C
 */
class C : public ModulePass
{
public:
  static char ID;
  C() : ModulePass(ID), data(0x12) { }
  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const {
    pUsage.addRequiredID(A::ID);
    pUsage.addRequiredID(B::ID);
  }

  StringRef getPassName() const { return "C"; }

  int data;
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

  const PassInfo* info = registry.getPassInfo(&C::ID);
  EXPECT_TRUE(info->getPassName().equals("C"));
  Pass* pass = info->makePass();
  EXPECT_EQ(((C*)pass)->data, 0x12);

  PassManager pm(registry);
  pm.add(pass);
  EXPECT_EQ(pm.size(), 3);

  pm.add(pass);
  // no additional passes
  EXPECT_EQ(pm.size(), 3);

  Module module;
  EXPECT_TRUE(pm.run(module));
}

SKYPAT_F(PassManagerTest, add_dependent_passes)
{
  PassRegistry registry;
  InitializeAPass(registry);

  PassManager pm(registry);
  pm.add(new B());
  EXPECT_EQ(pm.size(), 2);
  pm.add(new A());
  EXPECT_EQ(pm.size(), 2);
  pm.add(new C());
  EXPECT_EQ(pm.size(), 3);
}

class X : public ModulePass
{
public:
  static char ID;

  X() : ModulePass(ID) { }

  ReturnType runOnModule(Module &pModule) override { return kModuleChanged; }

  StringRef getPassName() const override { return "X"; }
};

char X::ID = 0;
INITIALIZE_PASS(X, "X")

class Y : public ModulePass
{
public:
  static char ID;

  Y() : ModulePass(ID) { }

  ReturnType runOnModule(Module &pModule) override { return kModuleNoChanged; }

  StringRef getPassName() const override { return "Y"; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequiredID(X::ID);
  }
};

char Y::ID = 0;
INITIALIZE_PASS(Y, "Y")

class Z : public ModulePass
{
public:
  static char ID;

  Z() : ModulePass(ID) { }

  ReturnType runOnModule(Module &pModule) override {
    static int c = 0;
    ++c;
    if (0 == c % 3)
      return kModuleChanged;

    // retry till c is 3
    return kPassRetry | kModuleChanged;
  }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequiredID(Y::ID);
  }

  StringRef getPassName() const override { return "Z"; }
};

char Z::ID = 0;
INITIALIZE_PASS(Z, "Z")

SKYPAT_F(PassManagerTest, run_test_1)
{
  PassRegistry registry;

  InitializeXPass(registry);
  InitializeYPass(registry);
  InitializeZPass(registry);
  
  ASSERT_EQ(registry.numOfPasses(), 3);
  ASSERT_FALSE(registry.isEmpty());

  PassManager::State state;
  PassManager pm(registry);
  pm.add(new Z(), state);
  pm.add(new Y(), state);
  pm.add(new Z(), state);
  ASSERT_EQ(state.execution.size(), 8); // XYZXYXYZ
  ASSERT_FALSE(state.changed);

  Module module;

  std::string process;

  pm.initRunState(module, state);

  // run X
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 7); // YZXYXYZ

  // run Y
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 6); // ZXYXYZ

  // run Z(1): retry
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 8); // XYZXYXYZ
  ASSERT_FALSE(state.changed);

  // run X: changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 7); // YZXYXYZ
  ASSERT_TRUE(state.changed);

  // run Y: no changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 6); // ZXYXYZ
  ASSERT_TRUE(state.changed);

  // run Z(2): retry
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 8); // XYZXYXYZ
  ASSERT_FALSE(state.changed);

  // run X: changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 7); // YZXYXYZ
  ASSERT_TRUE(state.changed);

  // run Y: no changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 6); // ZXYXYZ
  ASSERT_TRUE(state.changed);

  // run Z(3): changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 5); // XYXYZ
  ASSERT_TRUE(state.changed);

  // run X
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 4); // YXYZ

  // run Y
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 3); // XYZ

  // run X
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 2); // YZ

  // run Y: no changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 1); // Z
  ASSERT_TRUE(state.changed);

  // run Z(4): changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 3); // XYZ
  ASSERT_FALSE(state.changed);

  // run X: changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 2); // YZ
  ASSERT_TRUE(state.changed);

  // run Y: no changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 1); // Z
  ASSERT_TRUE(state.changed);

  // run Z(5): retry
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 3); // XYZ
  ASSERT_FALSE(state.changed);

  // run X: changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 2); // YZ
  ASSERT_TRUE(state.changed);

  // run Y: no changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 1); // Z
  ASSERT_TRUE(state.changed);

  // run Z(6): changed
  ASSERT_TRUE(pm.step(module, state));
  process += state.pass->getPassName();
  ASSERT_EQ(state.execution.size(), 0);
  ASSERT_TRUE(state.changed);

  errs() << process << std::endl;
  ASSERT_TRUE(process == "XYZXYZXYZXYXYZXYZXYZ");
  ASSERT_TRUE(pm.run(module, state));
}

// Testcase:
// P2 -> P1
// M1 -> P2
// P3 -> P2

class P1 : public ModulePass
{
public:
  static char ID;
  P1() : ModulePass(ID) { }
  ReturnType runOnModule(Module &pModule) override { return kModuleChanged; }
  StringRef getPassName() const override { return "P1 "; }
};

char P1::ID = 0;
INITIALIZE_PASS(P1, "P1")

class P2 : public ModulePass
{
public:
  static char ID;
  P2() : ModulePass(ID), data(0) { }
  ReturnType runOnModule(Module &pModule) override {
    data = -1;
    return kModuleNoChanged;
  }
  StringRef getPassName() const override { return "P2 "; }
  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequiredID(P1::ID);
  }
  int data;
};

char P2::ID = 0;
INITIALIZE_PASS(P2, "P2")

class M2 : public ModulePass
{
public:
  static char ID;
  M2() : ModulePass(ID) { }
  StringRef getPassName() const override { return "M2 "; }
  ReturnType runOnModule(Module &pModule) override {
    P2* p = getAnalysis<P2>();
    p->data = 12;
    return kModuleNoChanged;
  }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequiredID(P2::ID);
  }
};

char M2::ID = 0;
INITIALIZE_PASS(M2, "M2")

class P3 : public ModulePass
{
public:
  static char ID;
  P3() : ModulePass(ID) { }
  StringRef getPassName() const override { return "P3 "; }
  ReturnType runOnModule(Module &pModule) override { return kModuleNoChanged; }
  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequiredID(P2::ID);
  }
};

char P3::ID = 0;
INITIALIZE_PASS(P3, "P3")

SKYPAT_F(PassManagerTest, run_modifier_test)
{
  PassRegistry registry;

  InitializeP1Pass(registry);
  InitializeP2Pass(registry);
  InitializeP3Pass(registry);
  InitializeM2Pass(registry);

  PassManager::State state;
  PassManager pm(registry);

  // exe queue = P1 P2 M2 P1 P2 P3
  pm.add(new M2(), state);
  pm.add(new P3(), state);

  Module module;

  std::string process;

  pm.initRunState(module, state);

  // run P1
  ASSERT_TRUE(pm.step(module, state));
  ASSERT_TRUE(state.executed);
  process += state.pass->getPassName();

  // run P2
  ASSERT_TRUE(pm.step(module, state));
  ASSERT_TRUE(state.executed);
  process += state.pass->getPassName();

  // run M2
  ASSERT_TRUE(pm.step(module, state));
  ASSERT_TRUE(state.executed);
  process += state.pass->getPassName();

  ASSERT_TRUE(pm.step(module, state));
  ASSERT_FALSE(state.executed);

  ASSERT_TRUE(pm.step(module, state));
  ASSERT_FALSE(state.executed);

  // run P3
  ASSERT_TRUE(pm.step(module, state));
  ASSERT_TRUE(state.executed);
  process += state.pass->getPassName();

  P2* p2 = (P2*)pm.lookup(&P2::ID);
  ASSERT_EQ(p2->data, 12);

  errs() << process << std::endl;
  ASSERT_TRUE(process == "P1 P2 M2 P3 ");
}
