//===- EliminateIdentity.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Identity.h>
#include <onnc/Transforms/Optimizations/EliminateIdentity.h>

#include <vector>

using namespace onnc;

//===----------------------------------------------------------------------===//
// EliminateIdentity
//===----------------------------------------------------------------------===//
Pass::ReturnType EliminateIdentity::runOnModule(Module& pModule)
{
  Pass::ReturnType    ret = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg) {
    ret |= runOnComputeGraph(*cg->value());
  }

  if (ret != Pass::kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType EliminateIdentity::runOnComputeGraph(ComputeGraph& pCG)
{
  std::vector<ComputeOperator*> listOfIdentityNodes;
  for (ComputeOperator& node : pCG) {
    if (isa<Identity>(&node)) {
      listOfIdentityNodes.emplace_back(&node);
    }
  }

  // Early return
  if (listOfIdentityNodes.empty()) {
    return Pass::kModuleNoChanged;
  }

  // Erase nodes here
  for (auto* pIdentityNode : listOfIdentityNodes) {
    assert(pIdentityNode->getNumOfInputs() == 1 && "Identity must have exactly one input");
    assert(pIdentityNode->getNumOfOutputs() == 1 && "Identity must have exactly one output");

    Value* inV  = pIdentityNode->getInput(0);
    Value* outV = pIdentityNode->getOutput(0);
    outV->replaceAllUsesWith(*inV);

    pIdentityNode->removeAllInputs();
    pIdentityNode->removeAllOutputs();
    pCG.erase(*pIdentityNode);
  }

  return Pass::kModuleChanged;
}
