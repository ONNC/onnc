#include "TG.h"
#include "TGBackend.h"
#include <fstream>
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
  TGCodeEmit(TGBackend *pTarget, const std::string &pOutputFilename)
      : ModulePass(ID), m_Target(pTarget), m_OutputFilename(pOutputFilename)
  {
  }

  Pass::ReturnType runOnModule(Module &pModule) override
  {
    const ::onnx::Graph *graph = pModule.getGraphIR().get();
    TGCodeEmitter *CE = m_Target->getTargetCodeEmitter();
    CE->genWeightBin(m_OutputFilename + ".weight.bin");
    if (m_OutputFilename == "-") {
      CE->genRuntimeInfo(graph, onnc::outs());
      CE->encodeInstructions(onnc::outs());
    } else {
      std::fstream rt_fp(m_OutputFilename + ".rt.json",
                         std::ios::out | std::ios::binary);
      CE->genRuntimeInfo(graph, rt_fp);
      std::fstream asm_fp(m_OutputFilename + ".s", std::ios::out);
      CE->encodeInstructions(asm_fp);
    }
    return Pass::kModuleNoChanged;
  }

private:
  TGBackend *m_Target;
  std::string m_OutputFilename;
};

} // anonymous namespace

char TGCodeEmit::ID = 0;

ModulePass *onnc::createTGCodeEmitPass(TGBackend *pTarget,
                                       const std::string &pOutputFilename)
{
  return new TGCodeEmit(pTarget, pOutputFilename);
}
