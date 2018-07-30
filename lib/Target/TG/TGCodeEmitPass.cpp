//===- TGCodeEmitPass.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TGCodeEmitPass.h"
#include "TG.h"
#include "TGBackend.h"
#include <fstream>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

using namespace onnc;
using namespace onnc::TG;

char TGCodeEmit::ID = 0;

//===----------------------------------------------------------------------===//
// TGCodeEmit
//===----------------------------------------------------------------------===//
TGCodeEmit::TGCodeEmit(TGBackend *pTarget, const std::string &pOutputFilename)
    : ModulePass(ID), m_Target(pTarget), m_OutputFilename(pOutputFilename)
{
}

Pass::ReturnType TGCodeEmit::runOnModule(Module &pModule)
{
  const ::onnx::Graph *graph = pModule.getGraphIR().get();
  TGCodeEmitter *CE = m_Target->getTargetCodeEmitter();
  if (m_OutputFilename == "-") {
    CE->genRuntimeInfo(graph, onnc::outs());
    CE->encodeInstructions(onnc::outs());
  } else {
    // If we're printing in files, get weight binary first
    CE->genWeightBin(m_OutputFilename + ".weight.bin");
    std::fstream rt_fp(m_OutputFilename + ".rt.json",
                       std::ios::out | std::ios::binary);

    CE->genRuntimeInfo(graph, rt_fp);
    std::fstream asm_fp(m_OutputFilename + ".s", std::ios::out);
    CE->encodeInstructions(asm_fp);
  }
  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass *onnc::CreateTGCodeEmitPass(TGBackend *pTarget,
                                       const std::string &pOutputFilename)
{
  return new TGCodeEmit(pTarget, pOutputFilename);
}
