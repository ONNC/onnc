#include "TGFuseOptimizer.h"

using namespace onnc;

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
