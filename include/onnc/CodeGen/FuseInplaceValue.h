//===- FuseInplaceValue.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_FUSE_INPLACE_VALUE_H
#define ONNC_CODEGEN_FUSE_INPLACE_VALUE_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

class ComputeOperator;

/** \class FuseInplaceValue
 *  \brief If operator's input memory object can be directly reused by output,
 *         fuse input and output value into single value.
 *
 *  \note The pass break SSA form, the compute graph is no longer valid in
 *        terms of graph topology.
 */
class FuseInplaceValue : public ModulePass
{
public:
  static char ID;

  typedef bool (*IsFusible)(const ComputeOperator& pOp);

public:
  FuseInplaceValue(IsFusible pCheckFusibleFn = nullptr)
    : ModulePass(ID), m_IsFusibleFn(pCheckFusibleFn) {
  }

  StringRef getPassName() const override { return "FuseInplaceValue"; }

  Pass::ReturnType runOnModule(Module &pModule) override;

  Pass::ReturnType runOnComputeGraph(ComputeGraph& pCG);

private:
  IsFusible m_IsFusibleFn;
};

ModulePass*
CreateFuseInplaceValuePass(FuseInplaceValue::IsFusible pCheckFusibleFn);

} // namespace onnc

#endif