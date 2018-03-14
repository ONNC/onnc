#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "onnx/common/ir.h"
#include "TGBackend.h"

using namespace onnc;

namespace {

class TargetISel : public ModulePass {

private:
  TGBackend *m_target;
public:
  static char ID;
  TargetISel() : ModulePass(ID), m_target(nullptr) {}
  // FXIME
  // TargetISel(TGBackend* target) : ModulePass(ID), m_target(target) {}

  bool runOnModule(Module &pModule) override {
    onnx::Graph &graph = pModule.getGraph();
    TargetLowering *TLI = m_target->getTargetLowering();
    TLI->CodeGenAndEmitInst(&graph, m_target->getInsts());
    return false;
  }
};

} // anonymous namespace

char TargetISel::ID = 0;
INITIALIZE_PASS(TargetISel, "TargetISel")

// ModulePass *createRemoveUnusedNodePass() { return new TargetISel(); }
