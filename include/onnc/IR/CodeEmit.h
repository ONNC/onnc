//===- CodeEmitPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_CODE_EMIT_H
#define ONNC_IR_CODE_EMIT_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class CodeEmit : public CustomPass<CodeEmit>
{
public:
  CodeEmit(ComputeVisitor& pCodeEmitVisitor)
    : m_CodeEmitVisitor(pCodeEmitVisitor) {}

  StringRef getPassName() const override { return "CodeEmit"; }

  Pass::ReturnType runOnComputeGraph(ComputeGraph& pCG) override;
  
private:
  ComputeVisitor& m_CodeEmitVisitor;
};

CodeEmit* CreateCodeEmitPass(ComputeVisitor& pCodeEmitVisitor);

} // namespace of onnc


#endif 
