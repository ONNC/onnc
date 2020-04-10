//===- ReplaceFlattenByReshape.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ReplaceFlattenByReshape.h"

#include "NvDlaDefine.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>

namespace onnc {
namespace nvdla {
//===----------------------------------------------------------------------===//
// ReplaceFlattenByReshape
//===----------------------------------------------------------------------===//

const std::string ReplaceFlattenByReshape::shapePrefixName = "replace_flatten_by_reshape_shape_";
unsigned ReplaceFlattenByReshape::shapeIdx = 0;

Pass::ReturnType ReplaceFlattenByReshape::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

void ReplaceFlattenByReshape::replace_flatten_by_reshape(Flatten* flatten, ComputeGraph& pCG)
{
  Tensor* input_tensor = flatten->getInput(0);
  Tensor* output_tensor = flatten->getOutput(0);

  // Create a new reshape operator to replace
  Reshape* reshape = pCG.addOperator<Reshape>();

  // Create Int64Tensor for reshape input
  Initializer* shape_tensor_init = pCG.addOperator<Initializer>();
  Int64Tensor* shape_tensor = pCG.addValue<Int64Tensor>(shapePrefixName + std::to_string(shapeIdx++));
  assert(shape_tensor != nullptr && "The name must be unique");
  shape_tensor->setDimensions({static_cast<Tensor::Dimension>(output_tensor->getNumOfDimensions())});
  shape_tensor->getValues() = Int64Tensor::ValueList(output_tensor->getDimensions());
  
  shape_tensor_init->setTensor(*shape_tensor);

  // Delete edges
  flatten->removeAllInputs();
  flatten->removeAllOutputs();

  // Delete node
  pCG.erase(*flatten);

  // Connect input and output
  reshape->addInput(*input_tensor);
  reshape->addInput(*shape_tensor);
  reshape->addOutput(*output_tensor);
}

void ReplaceFlattenByReshape::remove_flatten(Flatten* flatten, ComputeGraph& pCG)
{
  // Set input_tensor as the previous operator's input
  assert(flatten->getOutput(0)->getUses().size() == 1);
  
  OutputOperator* output_operator = dyn_cast<OutputOperator>(flatten->getOutput(0)->getUses()[0].getUser());
  assert(output_operator);

  output_operator->removeAllInputs();
  output_operator->addInput(*flatten->getInput(0));

  // Delete edges
  flatten->removeAllInputs();
  flatten->removeAllOutputs();

  // Delete unused tensor
  pCG.erase(*flatten->getOutput(0));

  // Delete node
  pCG.erase(*flatten);
}

Pass::ReturnType ReplaceFlattenByReshape::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret   = Pass::kModuleNoChanged;
  std::vector<Flatten*> flattens_replace;
  std::vector<Flatten*> flattens_remove;

  // Find all flatten in graph
  for (auto& op : pCG) {
    if (Flatten* flatten = dyn_cast<Flatten>(&op)) {
      // Save flatten which needs to replace into a vector, is_remove_pattern() will handle case that needs to remove.
      if (is_remove_pattern(flatten)) {
        flattens_remove.push_back(flatten);
      }
      else {
        flattens_replace.push_back(flatten);
      }

      // Replacement will happen in the model, so this function should return kModuleChanged.
      ret |= Pass::kModuleChanged;
    }
  }

  // Replace flatten by reshape
  for (Flatten* flatten : flattens_replace) {
    replace_flatten_by_reshape(flatten, pCG);
  }

  // Remove flatten
  for (Flatten* flatten : flattens_remove) {
    remove_flatten(flatten, pCG);
  }

  if (Pass::kModuleChanged) {
    pCG.topologicalSort();
  }
  
  return ret;
}

bool ReplaceFlattenByReshape::is_remove_pattern(Flatten* flatten)
{
  // The output tensor of the Flatten has only one user.
  if (flatten->getOutput(0)->getUses().size() != 1) {
    return false;
  }

  // The user is OutputOperator
  OutputOperator* output_operator = dyn_cast<OutputOperator>(flatten->getOutput(0)->getUses()[0].getUser());
  if (output_operator == nullptr) {
    return false;
  }

  // Input dimension should in the format of NxCx1x1
  if (flatten->getInput(0)->getNumOfDimensions() != 4) {
    return false;
  }
  Tensor::Dimensions input_dims(flatten->getInput(0)->getDimensions());
  if (!(input_dims[2] == 1 && input_dims[3] == 1)) {
    return false;
  }

  // Output dimension should in the format of NxC
  if (flatten->getOutput(0)->getNumOfDimensions() != 2) {
    return false;
  }
  Tensor::Dimensions output_dims(flatten->getOutput(0)->getDimensions());
  if (output_dims[1] != input_dims[1]){
    return false;
  }

  return true;
}

} // namespace nvdla
} // namespace onnc
