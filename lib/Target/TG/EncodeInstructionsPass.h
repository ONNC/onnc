//===- EncodeInstructionsPass.h -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_ENCODE_INSTRUCTIONS_PASS_H
#define ONNC_TARGET_TG_ENCODE_INSTRUCTIONS_PASS_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {
class ComputeVisitor;

class EncodeInstructions : public ModulePass
{
public:
  static char ID;

public:
  EncodeInstructions(ComputeVisitor *pInstVisitor);

  StringRef getPassName() const override { return "EncodeInstructions"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  Pass::ReturnType runOnComputeGraph(::onnc::ComputeGraph &pCG);

private:
  ComputeVisitor *m_InstEmitVisitors;
};

ModulePass *CreateEncodeInstructionsPass(ComputeVisitor *pInstVisitor);

} // namespace onnc

#endif