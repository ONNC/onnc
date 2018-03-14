#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>
#include "TGBackend.h"

using namespace onnc;

namespace {

class TGCodeEmit : public ModulePass {

private:
  TGBackend *m_target;

public:
  static char ID;
  TGCodeEmit() : ModulePass(ID), m_target(nullptr) {}
  // TGCodeEmit(TGBackend *target) : ModulePass(ID), m_target(target) {}

  bool runOnModule(Module &pModule) override {
    m_target->codeEmit();
    return false;
  }

  void setTarget(TGBackend* target){
    m_target = target;
  }
};

} // anonymous namespace

char TGCodeEmit::ID = 0;
INITIALIZE_PASS(TGCodeEmit, "TGCodeEmit")

ModulePass *createTGCodeEmitPass(TGBackend *target) {
  auto pass = new TGCodeEmit();
  pass->setTarget(target);
  return pass;
}
