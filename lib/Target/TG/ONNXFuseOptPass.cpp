#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Dump.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class ONNXFuseOpt : public ModulePass
{

private:
  TGBackend *m_pTarget;

public:
  static char ID;

public:
  ONNXFuseOpt(TGBackend *pTarget) : ModulePass(ID), m_pTarget(pTarget) {}
  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  bool fuseNodes(::onnx::Graph *pGraph);
};

Pass::ReturnType ONNXFuseOpt::runOnModule(Module &pModule)
{
  ::onnx::Graph *graph = pModule.getGraphIR().get();
  Pass::ReturnType is_changed = Pass::kModuleNoChanged;
  bool local_change;
  do {
    local_change = fuseNodes(graph);
    if (local_change)
      is_changed = Pass::kModuleChanged;
  } while (local_change);
  DumpGraph(*graph);
  return is_changed;
}

bool ONNXFuseOpt::fuseNodes(::onnx::Graph *pGraph)
{
  for (auto it = pGraph->begin(), ie = pGraph->end(); it != ie; ++it) {
    auto *node = *it;
    auto symbol = node->kind();
    if (symbol == ::onnx::Symbol("Gemm")) {
      if (node->output()->uses().size() == 1 &&
          node->output()->uses()[0].user->kind() == ::onnx::Symbol("Relu")) {
        ::onnx::Node *relu_node = node->output()->uses()[0].user;
        ::onnx::Node *new_node =
            pGraph->create(::onnx::Symbol("Gemm_ReLU"), node->inputs());
        new_node->output()->copyMetadata(node->output());
        new_node->copyAttributes(*node);
        new_node->insertBefore(node);
        relu_node->replaceAllUsesWith(new_node);
        relu_node->destroy();
        node->destroy();
        return true;
      }
    }
  }
  return false;
}

} // namespace
char ONNXFuseOpt::ID = 0;

ModulePass *onnc::createONNXFuseOptPass(TGBackend *pTarget)
{
  return new ONNXFuseOpt(pTarget);
}
