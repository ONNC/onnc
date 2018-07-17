//===- CreateRemoveTrainingNodesPass.cpp ----------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/ModulePass.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnx/common/ir.h>

using namespace onnc;

RemoveTrainingNodes::RemoveTrainingNodes() : ModulePass(ID) {}

Pass::ReturnType RemoveTrainingNodes::runOnModule(::onnc::Module &pModule)
{
  ::onnx::Graph *graph = pModule.getRootTensorGraph();
  Pass::ReturnType isChanged = Pass::kModuleNoChanged;
  for (auto it = graph->begin(), ie = graph->end(); it != ie; ++it) {
    auto *node = *it;
    auto symbol = node->kind();
    if (symbol == ::onnx::Symbol("Dropout")) {
      // Dropout has multiple outputs
      node->outputs()[0]->replaceAllUsesWith(node->input());
      it.destroyCurrent();
      isChanged = Pass::kModuleChanged;
    }
  }
  return isChanged;
}

char RemoveTrainingNodes::ID = 0;

ModulePass *onnc::CreateRemoveTrainingNodesPass()
{
  return new RemoveTrainingNodes();
}