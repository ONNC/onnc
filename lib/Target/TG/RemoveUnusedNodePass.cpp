#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include "onnx/common/ir.h"
#include "TG.h"
#include "ONNXIRPrinter.h"

using namespace onnc;

namespace {

class RemoveUnusedNode : public ModulePass {

public:
  static char ID;

public:
  RemoveUnusedNode() : ModulePass(ID) {}

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override {
    ::onnx::Graph *graph = pModule.getGraph();
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

    // TODO use IR printer pass, or add passes by option (ex. printf-after-all)
    std::cout << "==================after RemoveUnusedNodePass: =====================" << std::endl;
    ONNXIRPrinter::dumpGraph(pModule.getGraphSP());

    return isChanged;
  }
};

} // anonymous namespace

char RemoveUnusedNode::ID = 0;

ModulePass *onnc::createRemoveUnusedNodePass() { return new RemoveUnusedNode(); }
