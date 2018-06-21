#include "TGFuseOptimizer.h"
#include "PatternMatch.h"

using namespace onnc;
using namespace PatternMatch;
bool TGFuseOptimizer::FuseNodes(onnx::Graph *pGraph)
{

  for (auto it = pGraph->begin(); it != pGraph->end(); ++it) {
    auto *node = *it;
    if (match(node, mSymbol("Gemm"), mFalseAttr("enableReLu")) and
        match(next(node), mSymbol("Relu"))) {
      FuseGemmRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Mul"), mAttr("axis", 1),
              mTrueAttr("broadcast")) and
        match(next(node), mSymbol("Add"), mAttr("axis", 1),
              mTrueAttr("broadcast"))) {
      FuseChannelWiseMulAdd(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("BatchNormalization")) and
        match(next(node), mSymbol("Scale"), mAttr("axis", 1),
              mTrueAttr("broadcast"))) {
      FuseBNScale(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Conv")) and
        match(next(node), mSymbol("Scale"), mAttr("axis", 1),
              mTrueAttr("broadcast"))) {
      FuseConvScale(pGraph, node, next(node));
      return true;
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

void TGFuseOptimizer::FuseChannelWiseMulAdd(onnx::Graph *pGraph,
                                            onnx::Node *pMulNode,
                                            onnx::Node *pAddNode)
{
  onnx::Node *scale_node =
      pGraph->create(onnx::Symbol("Scale"), pMulNode->inputs());
  scale_node->addInput(pAddNode->inputs()[1]);
  scale_node->output()->copyMetadata(pAddNode->output());
  scale_node->copyAttributes(*pMulNode);
  scale_node->insertBefore(pMulNode);
  pAddNode->replaceAllUsesWith(scale_node);
  pAddNode->destroy();
  pMulNode->destroy();
}

void TGFuseOptimizer::FuseBNScale(onnx::Graph *pGraph, onnx::Node *pBNNode,
                                  onnx::Node *pScaleNode)
{
  onnx::Node *scale_node = pGraph->create(onnx::Symbol("Scale"));
  // TODO recalculate weight by bn + scale -> scale
  scale_node->addInput(pBNNode->inputs()[0]);
  scale_node->addInput(pBNNode->inputs()[1]);
  scale_node->addInput(pBNNode->inputs()[2]);
  scale_node->output()->copyMetadata(pScaleNode->output());
  scale_node->copyAttributes(*pScaleNode);
  scale_node->insertBefore(pBNNode);
  pScaleNode->replaceAllUsesWith(scale_node);
  // remove unused initializer
  pGraph->eraseInitializer(pBNNode->inputs()[2]->uniqueName());
  pGraph->eraseInitializer(pBNNode->inputs()[3]->uniqueName());
  pGraph->eraseInitializer(pScaleNode->inputs()[0]->uniqueName());
  pGraph->eraseInitializer(pScaleNode->inputs()[1]->uniqueName());
  pScaleNode->destroy();
  pBNNode->destroy();
}

void TGFuseOptimizer::FuseConvScale(onnx::Graph *pGraph, onnx::Node *pConvNode,
                                    onnx::Node *pScaleNode)
{
  pConvNode->addInput(pScaleNode->inputs()[1]);
  pConvNode->addInput(pScaleNode->inputs()[2]);
  pConvNode->i_(onnx::Symbol("do_scale"), 1)
      ->i_(onnx::Symbol("do_scale_bias"), 1);
  pConvNode->output()->copyMetadata(pScaleNode->output());
  pScaleNode->replaceAllUsesWith(pConvNode);
  pScaleNode->destroy();
}
