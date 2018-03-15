#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "onnx/common/ir.h"
#include "TG.h"

using namespace onnc;

namespace {

class RemoveUnusedNode : public ModulePass {

public:
  static char ID;

public:
  RemoveUnusedNode() : ModulePass(ID) {}

  bool runOnModule(Module &pModule) override {
    onnx::Graph *graph = pModule.getGraph();
    bool isChanged = false;
    for (auto it = graph->begin(), ie = graph->end(); it != ie; ++it) {
      auto *node = *it;
      auto symbol = node->kind();
      if (symbol == onnx::Symbol("Dropout")) {
        // Dropout has multiple outputs
        node->outputs()[0]->replaceAllUsesWith(node->input());
        it.destroyCurrent();
        isChanged = true;
      }
    }
    return isChanged;
  }
};

} // anonymous namespace

char RemoveUnusedNode::ID = 0;

ModulePass *onnc::createRemoveUnusedNodePass() { return new RemoveUnusedNode(); }
