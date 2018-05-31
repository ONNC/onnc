#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class TGCodeEmit : public ModulePass
{
private:
  TGBackend *m_Target;

public:
  static char ID;

public:
  TGCodeEmit(TGBackend *pTarget) : ModulePass(ID), m_Target(pTarget) {}

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    ::onnx::Graph *graph = pModule.getGraphIR().get();
    m_Target->codeEmit(graph);
    return Pass::kModuleNoChanged;
  }

  void setTarget(TGBackend *pTarget) { m_Target = pTarget; }
};

} // anonymous namespace

char TGCodeEmit::ID = 0;

ModulePass *onnc::createTGCodeEmitPass(TGBackend *pTarget)
{
  return new TGCodeEmit(pTarget);
}
