//===- LinearScanAllocPass.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_LINEAR_SCAN_ALLOC_PASS_H
#define ONNC_TARGET_TG_LINEAR_SCAN_ALLOC_PASS_H
#include "BuildMemOpndPass.h"
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {
class TGBackend;

class LinearScanAlloc : public CustomPass<LinearScanAlloc>
{
public:
  LinearScanAlloc(TGBackend *pTarget);

  StringRef getPassName() const override { return "LinearScanAlloc"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

private:
  void linearScanAllocMem(const ComputeGraph &pCG,
                          const BuildMemOpnd::ValMemOpndMap &pValMemOpndMap);

  void memoryAlloc(ComputeMemOperand *pMemOp, const onnc::Value *pMemVal);

private:
  // for sizeOfTensorType.
  // FIXME: Use DLATargetBackend instead.
  //        sizeofTensorType is used on compute ir with legalized value type
  TGBackend *m_pTarget; // NOLINT
  unsigned int m_WeightOffset;
  unsigned int m_NeuronOffset;
  std::unordered_set<ComputeMemOperand*> m_AllocatedMemOpnd;
};

ModulePass *CreateLinearScanAllocPass(TGBackend *pTarget);

} // namespace onnc

#endif
