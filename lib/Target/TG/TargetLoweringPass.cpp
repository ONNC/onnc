#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>
#include "TGBackend.h"
#include "TG.h"

using namespace onnc;

namespace {

class TargetISel : public ModulePass
{
public:
  static char ID;

public:
  TargetISel(TGBackend* pBackend) : ModulePass(ID), m_pTarget(pBackend) {}

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
  {
    TargetLowering *TLI = m_pTarget->getTargetLowering();
    TLI->PrepareISelLowering(pModule);
    ::onnx::Graph *graph = pModule.getGraphIR().get();
    TLI->ISelLowering(graph);
    return Pass::kModuleNoChanged;
  }

private:
  TGBackend* m_pTarget;
};

} // anonymous namespace

char TargetISel::ID = 0;

ModulePass *onnc::createTargetLoweringPass(TGBackend *target) {
  return new TargetISel(target);
}
