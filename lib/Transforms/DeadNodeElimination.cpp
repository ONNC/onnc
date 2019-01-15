//===- DeadNodeElimination.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <iostream>

#include <onnc/Core/PassSupport.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Transforms/DeadNodeElimination.h>

using namespace onnc;

namespace {
  void dropOutputs(xNode& node)
  {
    while (0 < node.outputs().size()) {
      const auto lastIndex = node.outputs().size() - 1;
      auto* lastValue = node.outputs()[lastIndex];
      node.eraseOutput(lastIndex);
      delete lastValue;
    }
  }
} // anonymous namespace

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

  return Pass::kModuleChanged;
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
