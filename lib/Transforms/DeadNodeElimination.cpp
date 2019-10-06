//===- DeadNodeElimination.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <onnc/Core/PassSupport.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/Algorithm.h>
#include <onnc/Transforms/DeadNodeElimination.h>

#include <iostream>
#include <vector>

using namespace onnc;

//===----------------------------------------------------------------------===//
// DeadNodeElimination
//===----------------------------------------------------------------------===//
Pass::ReturnType DeadNodeElimination::runOnModule(::onnc::Module &pModule)
{
  xGraph* graph = pModule.getRootTensorGraph();

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    xNode* n = *it;
    // Remove 'undefined' node.
    if (n->kind() == xBuiltinSymbol::kUndefined) {
      dropOutputs(*n);
      it.destroyCurrent();
    }
  }

  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg) {
    runOnComputeGraph(*cg->value());
  }

  return Pass::kModuleNoChanged;
}

Pass::ReturnType DeadNodeElimination::runOnComputeGraph(ComputeGraph& graph)
{
  std::vector<ComputeOperator*> toRemove;
  for (ComputeOperator& op : graph) {
    if (isa<OutputOperator>(op)) {
      continue;
    }

    bool hasUser = false;
    for (unsigned idx = 0; idx < op.getNumOfOutputs(); ++idx) {
      const Value::UseList& users = op.getOutput(idx)->getUses();
      if (!empty(users)) {
        hasUser = true;
        break;
      }
    }

    if (!hasUser) {
      toRemove.emplace_back(&op);
    }
  }

  for (ComputeOperator* op : toRemove) {
    op->removeAllInputs();
    op->removeAllOutputs();
    graph.erase(*op);
  }

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(DeadNodeElimination, "DeadNodeElimination")
}

ModulePass* onnc::CreateDeadNodeEliminationPass()
{
  return new DeadNodeElimination();
}
