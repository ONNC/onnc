//===- CodeEmitPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_CODE_EMIT_H
#define ONNC_IR_CODE_EMIT_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class CodeEmit : public ModulePass
{
public:
  static char ID;

public:
  CodeEmit(ComputeVisitor& pCodeEmitVisitor)
    : ModulePass(CodeEmit::ID), m_CodeEmitVisitor(pCodeEmitVisitor) {}

  StringRef getPassName() const override { return "CodeEmit"; }

  Pass::ReturnType runOnModule(Module& pModule) override;

private:
  Pass::ReturnType runOnComputeGraph(ComputeGraph& pCG);
  
  ComputeVisitor& m_CodeEmitVisitor;
};

CodeEmit* CreateCodeEmitPass(ComputeVisitor& pCodeEmitVisitor);

} // namespace of onnc


#endif 
