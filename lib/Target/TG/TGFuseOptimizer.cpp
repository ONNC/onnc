#include "TGFuseOptimizer.h"

using namespace onnc;

bool TGFuseOptimizer::FuseNodes(onnx::Graph *pGraph)
{
  for (auto it = pGraph->begin(), ie = pGraph->end(); it != ie; ++it) {
    auto *node = *it;
    auto symbol = node->kind();
    if (symbol == ::onnx::Symbol("Gemm")) {
      if (node->output()->uses().size() == 1 &&
          node->output()->uses()[0].user->kind() == ::onnx::Symbol("Relu")) {
        ::onnx::Node *relu_node = node->output()->uses()[0].user;
        FuseGemmRelu(pGraph, node, relu_node);
        return true;
      }
    }
  }
  return false;
}

bool TGFuseOptimizer::FuseOptimization(onnx::Graph *pGraph)
{
  bool is_changed = false, local_changed;
  do {
    local_changed = FuseNodes(pGraph);
    is_changed |= local_changed;
  } while (local_changed);
  return is_changed;
}

void TGFuseOptimizer::FuseGemmRelu(::onnx::Graph *pGraph,
                                   ::onnx::Node *pGemmNode,
                                   ::onnx::Node *pReluNode)
{
  ::onnx::Node *new_node =
      pGraph->create(::onnx::Symbol("Gemm"), pGemmNode->inputs());
  new_node->output()->copyMetadata(pGemmNode->output());
  new_node->copyAttributes(*pGemmNode);
  new_node->i_(::onnx::Symbol("enableReLu"), 1);
  new_node->insertBefore(pGemmNode);
  pReluNode->replaceAllUsesWith(new_node);
  pReluNode->destroy();
  pGemmNode->destroy();
}
