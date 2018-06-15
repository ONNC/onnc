#include "TGFuseOptimizer.h"

using namespace onnc;

void TGFuseOptimizer::FuseGemmRelu(::onnx::Graph *pGraph,
                                   ::onnx::Node *pGemmNode,
                                   ::onnx::Node *pReluNode)
{
  ::onnx::Node *new_node =
      pGraph->create(::onnx::Symbol("Gemm_ReLU"), pGemmNode->inputs());
  new_node->output()->copyMetadata(pGemmNode->output());
  new_node->copyAttributes(*pGemmNode);
  new_node->insertBefore(pGemmNode);
  pReluNode->replaceAllUsesWith(new_node);
  pReluNode->destroy();
  pGemmNode->destroy();
}
