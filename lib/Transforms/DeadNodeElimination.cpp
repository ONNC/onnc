//===- DeadNodeElimination.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

#include <onnc/IR/ONNXUtils.h>
using namespace onnc;
#include <iostream>
//===----------------------------------------------------------------------===//
// DeadNodeElimination
//===----------------------------------------------------------------------===//
DeadNodeElimination::DeadNodeElimination()
  : ModulePass(ID) {
}

Pass::ReturnType DeadNodeElimination::runOnModule(::onnc::Module &pModule)
{
  ::onnx::Graph* graph = pModule.getRootTensorGraph();

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    ::onnx::Node* n = *it;
    // Remove 'undefined' node.
    if (n->kind() == ::onnx::kUndefined)
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