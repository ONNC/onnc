//===- DeadNodeElimination.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ONNXUtils.h>
#include <iostream>

using namespace onnc;

//===----------------------------------------------------------------------===//
// DeadNodeElimination
//===----------------------------------------------------------------------===//
DeadNodeElimination::DeadNodeElimination()
  : ModulePass(ID) {
}

Pass::ReturnType DeadNodeElimination::runOnModule(::onnc::Module &pModule)
{
  xGraph* graph = pModule.getRootTensorGraph();

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    xNode* n = *it;
    // Remove 'undefined' node.
    if (n->kind() == xBuiltinSymbol::kUndefined)
      it.destroyCurrent();
  }

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char DeadNodeElimination::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(DeadNodeElimination, "DeadNodeElimination")
}

ModulePass* onnc::CreateDeadNodeEliminationPass()
{
  return new DeadNodeElimination();
}
