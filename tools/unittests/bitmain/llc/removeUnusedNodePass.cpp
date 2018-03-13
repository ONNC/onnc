#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "onnx/common/ir.h"

using namespace onnc;

namespace {

class removeUnusedNode : public ModulePass {

public:
  static char ID;
  removeUnusedNode() : ModulePass(ID) {}

  bool runOnModule(Module &pModule) override {
    onnx::Graph &graph = pModule.getGraph();
    bool isChanged = false;
    for (auto it = graph.begin(), ie = graph.end(); it != ie; ++it) {
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

char removeUnusedNode::ID = 0;
INITIALIZE_PASS(removeUnusedNode, "removeUnusedNode")

// ModulePass *createRemoveUnusedNodePass() { return new removeUnusedNode(); }
