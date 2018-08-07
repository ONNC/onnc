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

using namespace onnc;
using namespace PatternMatch;

static inline bool isTensor(onnx::Value *pValue)
{
  onnx::Graph *graph = pValue->owningGraph();
  std::unordered_set<std::string> init_names(graph->initializer_names().begin(),
                                             graph->initializer_names().end());
  if (init_names.count(pValue->uniqueName()))
    return true;
  return false;
}

onnx::Node *TGFuseOptimizer::FuseMulAdd(onnx::Graph *pGraph,
                                        onnx::Node *pMulNode,
                                        onnx::Node *pAddNode)
{
  // create Scale node
  onnx::Node *scale_node = pGraph->create(onnx::Symbol("Scale"));
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

bool TGFuseOptimizer::FuseNodes(onnx::Graph *pGraph)
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
    if (match(node, mSymbol("Gemm")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Sum")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Conv")) and match(next(node), mSymbol("Scale"))) {
      FuseConvScale(pGraph, node, next(node));
      return true;
    }
  }
  return false;
}

bool TGFuseOptimizer::FuseOptimization(onnx::Graph *pGraph,
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

onnx::Node *TGFuseOptimizer::Fuse(::onnx::Node *pA, ::onnx::Node *pB)
{
  pB->replaceAllUsesWith(pA);
  pA->output()->copyMetadata(pB->output());
  pB->destroy();
  return pA;
}

onnx::Node *TGFuseOptimizer::FuseConvScale(onnx::Graph *pGraph,
                                           onnx::Node *pConvNode,
                                           onnx::Node *pScaleNode)
{
  pConvNode->addInput(pScaleNode->inputs()[1]);
  pConvNode->addInput(pScaleNode->inputs()[2]);
  pConvNode->i_(onnx::Symbol("do_scale"), 1)
      ->i_(onnx::Symbol("do_scale_bias"), 1);
  pConvNode->output()->copyMetadata(pScaleNode->output());
  pScaleNode->replaceAllUsesWith(pConvNode);
  pScaleNode->destroy();
  return pConvNode;
}

onnx::Node *TGFuseOptimizer::FuseRelu(onnx::Graph *pGraph, onnx::Node *pNode,
                                      onnx::Node *pReluNode)
{
  Fuse(pNode, pReluNode)->i_(::onnx::Symbol("do_relu"), 1);
  return pNode;
}
