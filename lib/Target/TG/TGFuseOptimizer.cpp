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

onnx::Node *TGFuseOptimizer::Fuse(::onnx::Node *pA, ::onnx::Node *pB)
{
  pB->replaceAllUsesWith(pA);
  pB->destroy();
  return pA;
}
void TGFuseOptimizer::FuseGemmRelu(::onnx::Graph *pGraph,
                                   ::onnx::Node *pGemmNode,
                                   ::onnx::Node *pReluNode)
{
  Fuse(pGemmNode, pReluNode)->i_(::onnx::Symbol("enableReLu"), 1);
}
