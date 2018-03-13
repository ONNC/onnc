#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "onnx/common/ir.h"
#include "TGBackend.h"

using namespace onnc;

namespace {

class TGCodeEmit : public ModulePass {

private:
  TGBackend *m_target;

public:
  static char ID;
  TGCodeEmit() : ModulePass(ID), m_target(nullptr) {}
  // FXIME
  // TGCodeEmit(TGBackend *target) : ModulePass(ID), m_target(target) {}

  bool runOnModule(Module &pModule) override {
    m_target->codeEmit();
    return false;
  }
};

} // anonymous namespace

char TGCodeEmit::ID = 0;
INITIALIZE_PASS(TGCodeEmit, "TGCodeEmit")

// ModulePass *createTGCodeEmitPass() { return new TGCodeEmit(); }
