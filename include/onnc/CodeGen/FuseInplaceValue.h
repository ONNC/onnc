//===- FuseInplaceValue.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_FUSE_INPLACE_VALUE_H
#define ONNC_CODEGEN_FUSE_INPLACE_VALUE_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

class ComputeOperator;

/** \class FuseInplaceValue
 *  \brief If operator's input memory object can be directly reused by output,
 *         fuse input and output value into single value.
 *
 *  \note The pass break SSA form, the compute graph is no longer valid in
 *        terms of graph topology.
 */
class FuseInplaceValue : public CustomPass<FuseInplaceValue>
{
public:
  typedef bool (*IsFusible)(const ComputeOperator& pOp);

public:
  FuseInplaceValue(IsFusible pCheckFusibleFn = nullptr)
    : m_IsFusibleFn(pCheckFusibleFn) {
  }

  StringRef getPassName() const override { return "FuseInplaceValue"; }

  Pass::ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  IsFusible m_IsFusibleFn;
};

} // namespace onnc

#endif
