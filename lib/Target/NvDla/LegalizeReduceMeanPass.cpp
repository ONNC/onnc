//===- LegalizeReduceMeanPass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LegalizeReduceMeanPass.h"

#include "NvDlaDefine.h"

#include <cassert>

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/ReduceMean.h>
#include <onnc/IR/Compute/Reshape.h>

namespace onnc {
namespace nvdla {
//===----------------------------------------------------------------------===//
// LegalizeReduceMeanPass
//===----------------------------------------------------------------------===//

const std::string LegalizeReduceMeanPass::shapePrefixName = "legalize_reducemean_shape_";
unsigned LegalizeReduceMeanPass::shapeIdx = 0;

Pass::ReturnType LegalizeReduceMeanPass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType LegalizeReduceMeanPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  std::vector<ReduceMean*> reducemeans;
  for (auto& op : pCG) {
    // Save all ReduceMean operators into vector
    if (ReduceMean* reducemean = dyn_cast<ReduceMean>(&op)) {
      if (is_replace_pattern(reducemean)){
        reducemeans.push_back(reducemean);
      }
    }
  }

  for (ReduceMean* op : reducemeans) {
    // Replace ReduceMean by GlobalAveragePool
    GlobalAveragePool* op_new = replace(op, pCG);

    if (op->getKeepdims() == 0) {
        // Reshape 1xCx1x1 into 1xC
        add_reshape(op_new, pCG);
    }

    ret |= Pass::kModuleChanged;
  }

  if (ret != kModuleNoChanged) {
    pCG.topologicalSort();
  }

  return ret;
}

GlobalAveragePool* LegalizeReduceMeanPass::replace(ReduceMean* reducemean, ComputeGraph& pCG)
{
  Tensor* input_tensor = reducemean->getInput(0);
  Tensor* output_tensor = reducemean->getOutput(0);

  // Create a new GlobalAveragePool operator to replace
  GlobalAveragePool* new_op = pCG.addOperator<GlobalAveragePool>();

  // Delete edges
  reducemean->removeAllInputs();
  reducemean->removeAllOutputs();

  // Delete node
  pCG.erase(*reducemean);

  // Connect input and output
  new_op->addInput(*input_tensor);
  new_op->addOutput(*output_tensor);

  return new_op;
}

void LegalizeReduceMeanPass::add_reshape(GlobalAveragePool* op, ComputeGraph& pCG)
{
  // Use this tensor as the output tensor of reshape
  Tensor* output_tensor = op->getOutput(0);

  // Create a new reshape operator
  Reshape* reshape = pCG.addOperator<Reshape>();

  // Create Int64Tensor for reshape to 1xC
  Initializer* shape_tensor_init = pCG.addOperator<Initializer>();
  Int64Tensor* shape_tensor = pCG.addValue<Int64Tensor>(shapePrefixName + std::to_string(shapeIdx++));
  assert(shape_tensor != nullptr && "The name must be unique");
  shape_tensor->setDimensions({2});
  shape_tensor->getValues() = Int64Tensor::ValueList({1, output_tensor->dimension(1)});
  shape_tensor_init->setTensor(*shape_tensor);

  // Create a FloatTensor as op(GlobalAveragePool)'s output 
  FloatTensor* op_output = pCG.addValue<FloatTensor>(shapePrefixName + std::to_string(shapeIdx++));
  assert(op_output != nullptr && "The name must be unique");
  op_output->setDimensions({1, output_tensor->dimension(1), 1, 1});

  // GlobalAveragePool should has only one output
  assert(op->getNumOfOutputs() == 1);
  op->removeAllOutputs();
  op->addOutput(*op_output);

  reshape->addInput(*op_output);
  reshape->addInput(*shape_tensor);
  reshape->addOutput(*output_tensor);
}

bool LegalizeReduceMeanPass::is_replace_pattern(ReduceMean* op)
{
  // Axes should be {2, 3}
  if (!(op->getAxes().vector() == IntsAttr::VectorType({2, 3}))) {
    return false;
  }

  // Input tensor should be NxCxHxW
  if (!(op->getInput(0)->getNumOfDimensions() == 4)) {
    return false;
  }

  // The pattern matches, so we need to remove this pad
  return true;
}

} // namespace nvdla
} // namespace onnc
