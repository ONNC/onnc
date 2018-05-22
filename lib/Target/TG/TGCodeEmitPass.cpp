#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>
#include "TGBackend.h"
#include "TG.h"

using namespace onnc;

namespace {

class TGCodeEmit : public ModulePass
{
private:
  TGBackend *m_target;

public:
  static char ID;

public:
  TGCodeEmit(TGBackend *target) : ModulePass(ID), m_target(target) {}

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    m_target->codeEmit();
    return Pass::kModuleNoChanged;
  }

  void setTarget(TGBackend* target){
    m_target = target;
  }
};

} // anonymous namespace

char TGCodeEmit::ID = 0;

ModulePass *onnc::createTGCodeEmitPass(TGBackend *target) {
  return new TGCodeEmit(target);
}
