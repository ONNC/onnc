#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class TGCodeEmit : public ModulePass
{
public:
  static char ID;

public:
  TGCodeEmit(TGBackend *pTarget, const Path &pOutputPath)
      : ModulePass(ID), m_Target(pTarget), m_OutputPath(pOutputPath)
  {
  }

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    const ::onnx::Graph *graph = pModule.getGraphIR().get();
    TGCodeEmitter *CE = m_Target->getTargetCodeEmitter();
    CE->genRuntimeInfo(graph);
    CE->encodeInstructions(m_OutputPath);
    return Pass::kModuleNoChanged;
  }

private:
  TGBackend *m_Target;
  Path m_OutputPath;
};

} // anonymous namespace

char TGCodeEmit::ID = 0;

ModulePass *onnc::createTGCodeEmitPass(TGBackend *pTarget,
                                       const Path &pOutputPath)
{
  return new TGCodeEmit(pTarget, pOutputPath);
}
