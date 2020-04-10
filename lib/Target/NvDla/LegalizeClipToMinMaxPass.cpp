//===- LegalizeClipToMinMaxPass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LegalizeClipToMinMaxPass.h"

#include "NvDlaDefine.h"

#include <cassert>

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>

namespace onnc {
namespace nvdla {
//===----------------------------------------------------------------------===//
// LegalizeClipToMinMaxPass
//===----------------------------------------------------------------------===//

unsigned LegalizeClipToMinMaxPass::clipIdx = 0;

Pass::ReturnType LegalizeClipToMinMaxPass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType LegalizeClipToMinMaxPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  std::vector<Clip*> clips;

  for (auto& op : pCG) {
    if (!isa<Clip>(op)) {
      continue;
    }

    Clip* clip = cast<Clip>(&op);
    clips.push_back(clip);

    ret |= Pass::kModuleChanged;
  }

  for (Clip* clip : clips) {
    legalize(clip, pCG);
  }

  if (ret != kModuleNoChanged) {
    pCG.topologicalSort();
  }

  return ret;
}

void LegalizeClipToMinMaxPass::legalize(Clip* clip, ComputeGraph& pCG)
{
  Tensor* const input = clip->getInput();
  Tensor* const output = clip->getOutput();
  const FloatAttr max_value = clip->getMax();
  const FloatAttr min_value = clip->getMin();

  // Create Tensor for upper bound
  Initializer* max_tensor_init = pCG.addOperator<Initializer>();
  FloatTensor* max_tensor = pCG.addValue<FloatTensor>(input->getName() + "_max_" + std::to_string(clipIdx++));
  assert(max_tensor != nullptr && "The name must be unique");
  max_tensor->setDimensions({1});
  max_tensor->getValues().emplace_back(max_value);
  max_tensor_init->setTensor(*max_tensor);

  // Create Tensor for lower bound
  Initializer* min_tensor_init = pCG.addOperator<Initializer>();
  FloatTensor* min_tensor = pCG.addValue<FloatTensor>(input->getName() + "_min_" + std::to_string(clipIdx++));
  assert(min_tensor != nullptr && "The name must be unique");
  min_tensor->setDimensions({1});
  min_tensor->getValues().emplace_back(min_value);
  min_tensor_init->setTensor(*min_tensor);

  // Create Tensor for output of Max
  Tensor* mid_tensor = input->clone();
  mid_tensor->setName(mid_tensor->getName() + "_middle_" + std::to_string(clipIdx++));
  mid_tensor = pCG.addValue<Tensor>(mid_tensor);
  assert(mid_tensor != nullptr && "The name must be unique");

  // Delete edges
  clip->removeAllInputs();
  clip->removeAllOutputs();

  // Delete node
  pCG.erase(*clip);

  // Create new Max and Min operator to replace Clip
  Max* max = pCG.addOperator<Max>();
  Min* min = pCG.addOperator<Min>();

  // Connect new operator
  max->addInput(*input);
  max->addInput(*min_tensor);
  max->addOutput(*mid_tensor);

  min->addInput(*mid_tensor);
  min->addInput(*max_tensor);
  min->addOutput(*output);
}

} // namespace nvdla
} // namespace onnc
