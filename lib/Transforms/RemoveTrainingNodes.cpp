//===- CreateRemoveTrainingNodesPass.cpp ----------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/ONNX.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>

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

Pass::ReturnType RemoveTrainingNodes::runOnModule(::onnc::Module &pModule)
{
  xGraph *graph = pModule.getRootTensorGraph();
  Pass::ReturnType isChanged = Pass::kModuleNoChanged;
  for (auto it = graph->begin(); it != graph->end(); ++it) {
    auto *node = *it;
    auto symbol = node->kind();
    if (symbol == xSymbol("Dropout")) {
      isChanged = Pass::kModuleChanged;

      node->outputs()[0]->replaceAllUsesWith(node->input());

      dropOutputs(*node);

      it.destroyCurrent();
    }
  }
  return isChanged;
}

ModulePass *onnc::CreateRemoveTrainingNodesPass()
{
  return new RemoveTrainingNodes();
}
