//===- EncodeInstructionsPass.h -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_ENCODE_INSTRUCTIONS_PASS_H
#define ONNC_TARGET_TG_ENCODE_INSTRUCTIONS_PASS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {
class ComputeVisitor;

class EncodeInstructions : public CustomPass<EncodeInstructions>
{
public:
  explicit EncodeInstructions(ComputeVisitor *pInstVisitor);

  StringRef getPassName() const override { return "EncodeInstructions"; }

  Pass::ReturnType runOnComputeGraph(::onnc::ComputeGraph &pCG) override;

protected:
  virtual void beforeEmit(const ::onnc::ComputeOperator* pOp) {}

private:
  ComputeVisitor *m_InstEmitVisitors;
};

ModulePass *CreateEncodeInstructionsPass(ComputeVisitor *pInstVisitor);

} // namespace onnc

#endif
