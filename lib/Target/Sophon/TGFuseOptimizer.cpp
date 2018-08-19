//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "TGFuseOptimizer.h"
#include "PatternMatch.h"
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;
using namespace PatternMatch;

static inline bool isTensor(xValue *pValue)
{
  xGraph *graph = pValue->owningGraph();
  std::unordered_set<std::string> init_names(graph->initializer_names().begin(),
                                             graph->initializer_names().end());
  if (init_names.count(pValue->uniqueName()))
    return true;
  return false;
}

xNode *TGFuseOptimizer::FuseMulAdd(xGraph *pGraph,
                                   xNode *pMulNode,
                                   xNode *pAddNode)
{
  // create Scale node
  xNode *scale_node = pGraph->create(xSymbol("TGScale"));
  scale_node->addInput(pMulNode->inputs()[0]);
  scale_node->addInput(pMulNode->inputs()[1]);
  scale_node->addInput(pAddNode->inputs()[1]);
  scale_node->output()->copyMetadata(pAddNode->output());
  scale_node->insertBefore(pMulNode);

  pAddNode->replaceAllUsesWith(scale_node);
  pAddNode->destroy();
  pMulNode->destroy();
  return scale_node;
}

bool TGFuseOptimizer::FuseNodes(xGraph *pGraph)
{
  for (auto it = pGraph->begin(); it != pGraph->end(); ++it) {
    auto *node = *it;

    if (match(node, mSymbol("Mul")) && match(next(node), mSymbol("Add")) &&
        isTensor(node->inputs()[1]) && isTensor(next(node)->inputs()[1])) {
      auto mul_input_dims = node->inputs()[0]->sizes();
      auto add_input_dims = next(node)->inputs()[0]->sizes();
      auto mul_tensor_dims = node->inputs()[1]->sizes();
      auto add_tensor_dims = next(node)->inputs()[1]->sizes();
      // check this is channel-wise Mul and Add
      // C = Mul (A, B) // A is (n,c,h,w), B is (1,c,1,1)
      // E = Add (C, D) // C is (n,c,h,w), D is (1,c,1,1)
      if (mul_input_dims.size() == mul_tensor_dims.size() &&
          mul_tensor_dims.size() == add_tensor_dims.size() &&
          mul_input_dims.size() == 4 && add_input_dims.size() == 4 &&
          mul_input_dims[1].dim == add_input_dims[1].dim &&
          mul_tensor_dims[0].dim == 1 &&
          mul_tensor_dims[1].dim == mul_input_dims[1].dim &&
          mul_tensor_dims[2].dim == 1 && mul_tensor_dims[3].dim == 1 &&
          add_tensor_dims[0].dim == 1 &&
          add_tensor_dims[1].dim == add_input_dims[1].dim &&
          add_tensor_dims[2].dim == 1 && add_tensor_dims[3].dim == 1) {
        FuseMulAdd(pGraph, node, next(node));
        return true;
      }
    }
    if (match(node, mSymbol("Conv")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("TGConv")) and
        match(next(node), mSymbol("Relu"), mFalseAttr("do_scale"),
              mFalseAttr("do_scale_bias"))) {
      TGFuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Gemm")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Sum")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Conv")) and
        match(next(node), mSymbol("TGScale"))) {
      FuseConvScale(pGraph, node, next(node));
      return true;
    }
  }
  return false;
}

bool TGFuseOptimizer::FuseOptimization(xGraph *pGraph,
                                       const int64_t &pOpsetVersion)
{
  if (pOpsetVersion != 7 && pOpsetVersion != 8) {
    std::cerr << "error: unsupported opset version optimization!" << std::endl;
    return false;
  }

  bool is_changed = false, local_changed;
  do {
    local_changed = FuseNodes(pGraph);
    is_changed |= local_changed;
  } while (local_changed);
  return is_changed;
}

xNode *TGFuseOptimizer::Fuse(xNode *pA, xNode *pB)
{
  pB->replaceAllUsesWith(pA);
  pA->output()->copyMetadata(pB->output());
  pB->destroy();
  return pA;
}

xNode *TGFuseOptimizer::FuseConvScale(xGraph *pGraph,
                                           xNode *pConvNode,
                                           xNode *pScaleNode)
{
  auto *new_conv = pGraph->create(xSymbol("TGConv"), pConvNode->inputs());
  new_conv->addInput(pScaleNode->inputs()[1]);
  new_conv->addInput(pScaleNode->inputs()[2]);
  new_conv->copyAttributes(*pConvNode);
  new_conv->i_(xSymbol("do_scale"), 1)
      ->i_(xSymbol("do_scale_bias"), 1);
  new_conv->output()->copyMetadata(pScaleNode->output());
  new_conv->insertBefore(pConvNode);
  pScaleNode->replaceAllUsesWith(new_conv);
  pScaleNode->destroy();
  pConvNode->destroy();
  return new_conv;
}

xNode *
TGFuseOptimizer::FuseRelu(xGraph *pGraph, xNode *pNode, xNode *pReluNode)
{
  std::string symbol_str = std::string("TG") + pNode->kind().toString();
  auto *new_node = pGraph->create(xSymbol(symbol_str), pNode->inputs());
  new_node->copyAttributes(*pNode);
  new_node->output()->copyMetadata(pReluNode->output());
  new_node->i_(xSymbol("do_relu"), 1);
  new_node->insertBefore(pNode);
  pReluNode->replaceAllUsesWith(new_node);
  pReluNode->destroy();
  pNode->destroy();
  return new_node;
}

xNode *TGFuseOptimizer::TGFuseRelu(xGraph *pGraph, xNode *pNode,
                                   xNode *pReluNode)
{
  pNode->i_(xSymbol("do_relu"), 1);
  pNode->output()->copyMetadata(pReluNode->output());
  pReluNode->replaceAllUsesWith(pNode);
  pReluNode->destroy();
  return pNode;
}
