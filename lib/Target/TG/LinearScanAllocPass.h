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
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {
class TGBackend;

class LinearScanAlloc : public ModulePass
{
public:
  static char ID;

public:
  LinearScanAlloc(TGBackend *pTarget);

  StringRef getPassName() const override { return "LinearScanAlloc"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

private:
  void linearScanAlloMem(const BuildMemOpnd::MemOperandValList &pMemOps);

private:
  // for sizeOfTensorType.
  // FIXME: Use DLATargetBackend instead.
  //        sizeofTensorType is used on compute ir with legalized value type
  TGBackend *m_pTarget; // NOLINT
};

ModulePass *CreateLinearScanAllocPass(TGBackend *pTarget);

} // namespace onnc

#endif
