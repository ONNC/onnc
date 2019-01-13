//===- AddLutTablePass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_SOPHON_BM188X_ADD_LUT_TABLE_PASS_H
#define ONNC_TARGET_SOPHON_BM188X_ADD_LUT_TABLE_PASS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnc/Transforms/GraphBuildingPass.h>
#include <onnc/Config/ONNX.h>

namespace onnc {
namespace BM188X {

class AddLutTablePass : public CustomPass<AddLutTablePass, GraphBuildingPass>
{
public:
  AddLutTablePass(onnc::DLATargetBackend* pBackend)
    : m_pTarget(pBackend)
  {
  }

  ~AddLutTablePass() {}

  Pass::ReturnType runOnGraphs(xGraph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "AddLutTablePass"; }

private:
  DLATargetBackend *m_pTarget; // NOLINT
};

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass* CreateAddLutTablePass(DLATargetBackend* pBackend);

} // namespace BM188X
} // namespace onnc

#endif
