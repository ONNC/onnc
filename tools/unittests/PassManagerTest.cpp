//===- PassManagerTest.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Core/Pass.h>
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassRegistry.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassManager.h>
#include <onnc/IR/Module.h>

#include <memory>

using namespace skypat;
using namespace onnc;

namespace {

/** \class A
 */
class A : public CustomPass<A>
{
public:
  A() = default;

  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  StringRef getPassName() const { return "A"; }
};

/** \class B
 */
class B : public CustomPass<B>
{

public:
  B() = default;

  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const {
    pUsage.addRequired<A>();
  }

  StringRef getPassName() const { return "B"; }
};

/** \class C
 */
class C : public CustomPass<C>
{
public:
  C() : data(0x12) { }
  ReturnType runOnModule(Module &pModule) { return kModuleChanged; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const {
    pUsage.addRequired<A>();
    pUsage.addRequired<B>();
  }

  StringRef getPassName() const { return "C"; }

  int data;
};

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

  const PassInfo* info = registry.getPassInfo(C::id());
  EXPECT_TRUE(info->getPassName().equals("C"));
  const auto pass = std::unique_ptr<Pass>{info->makePass()};
  EXPECT_EQ(static_cast<const C*>(pass.get())->data, 0x12);

  PassManager pm(registry);
  pm.add<C>();
  EXPECT_EQ(pm.size(), 3);

  pm.add<C>();
  EXPECT_EQ(pm.size(), 4);

  Module module;
  EXPECT_TRUE(pm.run(module));
}

SKYPAT_F(PassManagerTest, add_dependent_passes)
{
  PassRegistry registry;
  InitializeAPass(registry);

  PassManager pm(registry);
  pm.add<B>();
  EXPECT_EQ(pm.size(), 2);
  pm.add<A>();
  EXPECT_EQ(pm.size(), 3);
  pm.add<C>();
  EXPECT_EQ(pm.size(), 4);
}

class X : public CustomPass<X>
{
public:
  X() = default;

  ReturnType runOnModule(Module &pModule) override { return kModuleChanged; }

  StringRef getPassName() const override { return "X"; }
};

INITIALIZE_PASS(X, "X")

class Y : public CustomPass<Y>
{
public:
  Y() = default;

  ReturnType runOnModule(Module &pModule) override { return kModuleNoChanged; }

  StringRef getPassName() const override { return "Y"; }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequired<X>();
  }
};

INITIALIZE_PASS(Y, "Y")

class Z : public CustomPass<Z>
{
public:
  Z() = default;

  ReturnType runOnModule(Module &pModule) override {
    static int c = 0;
    ++c;
    if (0 == c % 3)
      return kModuleChanged;

    // retry till c is 3
    return kPassRetry | kModuleChanged;
  }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequired<Y>();
  }

  StringRef getPassName() const override { return "Z"; }
};

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

class P1 : public CustomPass<P1>
{
public:
  P1() = default;
  ReturnType runOnModule(Module &pModule) override { return kModuleChanged; }
  StringRef getPassName() const override { return "P1 "; }
};

INITIALIZE_PASS(P1, "P1")

class P2 : public CustomPass<P2>
{
public:
  P2() : data(0) { }
  ReturnType runOnModule(Module &pModule) override {
    data = -1;
    return kModuleNoChanged;
  }
  StringRef getPassName() const override { return "P2 "; }
  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequired<P1>();
  }
  int data;
};

INITIALIZE_PASS(P2, "P2")

class M2 : public CustomPass<M2>
{
public:
  M2() = default;
  StringRef getPassName() const override { return "M2 "; }
  ReturnType runOnModule(Module &pModule) override {
    P2* p = getAnalysis<P2>();
    p->data = 12;
    return kModuleNoChanged;
  }

  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequired<P2>();
  }
};

INITIALIZE_PASS(M2, "M2")

class P3 : public CustomPass<P3>
{
public:
  P3() = default;
  StringRef getPassName() const override { return "P3 "; }
  ReturnType runOnModule(Module &pModule) override { return kModuleNoChanged; }
  void getAnalysisUsage(AnalysisUsage& pUsage) const override {
    pUsage.addRequired<P2>();
  }
};

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

  P2* p2 = (P2*)pm.getPass(P2::id());
  ASSERT_EQ(p2->data, 12);

  errs() << process << std::endl;
  ASSERT_TRUE(process == "P1 P2 M2 P3 ");
}
