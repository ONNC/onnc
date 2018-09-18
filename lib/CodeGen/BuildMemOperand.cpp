//===- BuildMemOperandPass.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Cast.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BuildMemOperand
//===----------------------------------------------------------------------===//
Pass::ReturnType BuildMemOperand::runOnModule(Module& pModule)
{
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    createMemOperandsOfGraph(*cg->value());

  return Pass::kModuleNoChanged;
}

void BuildMemOperand::createMemOperandsOfGraph(ComputeGraph& pCG)
{
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator *node = nodeIt;
    ComputeOperand::Residence resd;
    if (isa<Initializer>(node))
      resd = ComputeOperand::kWeightResidence;
    else if (isa<InputOperator>(node))
      resd = ComputeOperand::kInputResidence;
    else if (isa<OutputOperator>(node))
      resd = ComputeOperand::kOutputResidence;
    else
      resd = ComputeOperand::kInternalResidence;

    createMemOperandsOfNode(pCG, *node, resd);
  }
}

void BuildMemOperand::createMemOperandsOfNode(ComputeGraph& pCG,
                                              ComputeOperator& pNode,
                                              ComputeOperand::Residence pResd)
{
  // Create memory operand for each output value
  for (unsigned i = 0; i < pNode.getNumOfOutputs(); ++i) {
    onnc::Value* value = pNode.getOutput(i);
    for (auto& use : value->getUses()) {
      pCG.addOperand<ComputeMemOperand>(pNode, *use.getUser(),
                                        *value, pResd);

    }
  }
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char BuildMemOperand::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(BuildMemOperand, "BuildMemOperand")
}

ModulePass* onnc::CreateBuildMemOperandPass()
{
  return new BuildMemOperand();
}