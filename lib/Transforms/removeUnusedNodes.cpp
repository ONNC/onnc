#include <onnc/Core/ModulePass.h>
#include <onnc/Transforms/removeUnusedNodes.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class RemoveUnusedNodes : public ModulePass
{

public:
  static char ID;

public:
  RemoveUnusedNodes() : ModulePass(ID) {}

  StringRef getPassName() const override { return "RemoveUnusedNodes"; }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
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
};

} // anonymous namespace

char RemoveUnusedNodes::ID = 0;

ModulePass *onnc::createRemoveUnusedNodesPass()
{
  return new RemoveUnusedNodes();
}
