//===- ExpandBatchNormalization.h -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_EXPAND_BATCHNORMALIZATION_H_INCLUDED
#define ONNC_EXPAND_BATCHNORMALIZATION_H_INCLUDED

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/BatchNormalization.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Module.h>

namespace onnc {

/** \class ExpandBatchNormalization
 *  \brief Expand Batch Normalization to Mul and Add 
 */
class ExpandBatchNormalization: public CustomPass<ExpandBatchNormalization>
{
public:
  ExpandBatchNormalization() = default;

  ReturnType runOnModule(Module &pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph &pCG) override;

  void expandBNToAddAndMul(ComputeGraph& pCG, BatchNormalization& batchNormalization);

private:
  static unsigned tensorIdx;
};

} // namespace of onnc

#endif // ONNC_EXPAND_BATCHNORMALIZATION_H_INCLUDED
