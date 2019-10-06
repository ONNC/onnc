//===- NvDlaIdentifyShufflePass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaIdentifyShufflePass.h"

#include "Compute/NvDlaShuffle.h"
#include "NvDlaDefine.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Transpose.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// NvDlaIdentifyShufflePass
//===----------------------------------------------------------------------===//
Pass::ReturnType NvDlaIdentifyShufflePass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret   = Pass::kModuleNoChanged;
  ComputeGraph*    graph = pModule.getRootComputeGraph();

  std::vector<Reshape*> reshapes;
  for (auto& op : *graph) {
    if (Reshape* reshape1 = dyn_cast<Reshape>(&op)) {
      if (is_shuffle(reshape1)) {
        reshapes.push_back(reshape1);
      }
    }
  }

  for (Reshape* reshape1 : reshapes) {
    auto* creator  = static_cast<ComputeOperator*>(reshape1->getInput(0)->getDefine());
    auto* trans    = dyn_cast<Transpose>(reshape1->getOutput(0)->getUses()[0].getUser());
    auto* reshape2 = dyn_cast<Reshape>(trans->getOutput(0)->getUses()[0].getUser());

    const auto& reshape_shape = static_cast<Int64Tensor*>(reshape1->getInput(1))->getValues();
    auto*       shuffle       = graph->addOperator<NvDlaShuffle>(1, reshape_shape[1]);
    Value*      emptyV        = new Value;
    shuffle->addInput(*reshape1->getInput(0));
    reshape1->removeAllInputs();

    reshape2->getOutput(0)->clearDefine();
    shuffle->addOutput(*reshape2->getOutput(0));

    auto* reshap1_input_shape = static_cast<ComputeOperator*>(reshape1->getInput(1)->getDefine());
    auto* reshap2_input_shape = static_cast<ComputeOperator*>(reshape2->getInput(1)->getDefine());
    graph->erase(*reshap1_input_shape);
    graph->erase(*reshap2_input_shape);
    graph->erase(*reshape1);
    graph->erase(*trans);
    graph->erase(*reshape2);
  }

  graph->topologicalSort();

  return ret;
}

bool NvDlaIdentifyShufflePass::is_shuffle(Reshape* shape1)
{
  if (shape1->getNumOfOutputs() != 1)
    return false;
  if (shape1->getOutput(0)->getUses().size() != 1)
    return false;

  ComputeOperator* shape1_use = shape1->getOutput(0)->getUses()[0].getUser();

  Transpose* trans;
  Reshape*   shape2;
  if (trans = dyn_cast<Transpose>(shape1_use)) { // assert shape1 -> trans
    if (trans->getNumOfOutputs() != 1)
      return false;
    if (trans->getOutput(0)->getUses().size() != 1)
      return false;

    ComputeOperator* trans1_use = trans->getOutput(0)->getUses()[0].getUser();

    // check ok: no branch between  (value*) -> shape1 -> trans -> shape2 -> (uses)
    if (shape2 = dyn_cast<Reshape>(trans1_use)) {
      // check dimension

      // input tensor dim == 4 (e.g, 1 * 56 * 23 * 2)
      if (shape1->getInput(0)->getNumOfDimensions() != 4)
        return false;
      // new shape dim == 1    (e.g, [])
      if (shape1->getInput(1)->getNumOfDimensions() != 1)
        return false;

      const auto& reshape_shape = static_cast<Int64Tensor*>(shape1->getInput(1))->getValues();
      if (reshape_shape.size() != 5)
        return false;
      if (shape1->getInput(0)->dimension(2) != reshape_shape[3] or
          shape1->getInput(0)->dimension(3) != reshape_shape[4])
        return false;

      // transpose input dim == 5 (e.g, 1 * 2 * 28 * 23 * 2)
      if (trans->getInput(0)->getNumOfDimensions() != 5)
        return false;
      // swap 1 and 2             (e.g, 1 * 28 * 2 * 23 * 2)
      if (trans->getPerm().at(1) != 2 or trans->getPerm().at(2) != 1 or trans->getPerm().at(0) != 0 or
          trans->getPerm().at(3) != 3 or trans->getPerm().at(4) != 4)
        return false;

      const auto& reshape_shape2 = static_cast<Int64Tensor*>(shape2->getInput(1))->getValues();
      // input tensor dime == 5   (e.g, 1 * 28 * 2 * 23 * 2)
      if (shape2->getInput(0)->getNumOfDimensions() != 5)
        return false;
      // output tensor dim == 4   (e.g, 1 * 56 * 23 * 2)
      if (reshape_shape2.size() != 4)
        return false;

      for (int i = 0; i < 4; i++) {
        if (shape1->getInput(0)->dimension(i) != reshape_shape2[i])
          return false;
      }
    } else
      return false;
  } else
    return false;

  return true;
}
