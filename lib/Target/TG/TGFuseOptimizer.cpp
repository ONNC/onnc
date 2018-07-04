#include "TGFuseOptimizer.h"
#include "PatternMatch.h"
#include <onnc/IR/ONNXUtils.h>

using namespace onnc;
using namespace PatternMatch;

static std::vector<float> getTensorData(onnx::Graph *pGraph,
                                        const std::string &pName)
{
  const onnx::Tensor &tensor = *pGraph->getInitializer(pName);
  size_t count = onnc::getTotalCount(tensor.sizes());
  assert(tensor.is_raw_data());
  const std::string &raw = tensor.raw();
  std::vector<float> data(count);
  std::memcpy(data.data(), raw.data(), count * sizeof(float));
  return data;
}

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
    if (match(node, mSymbol("Conv")) and match(next(node), mSymbol("Relu"))) {
      FuseConvRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Sum")) and match(next(node), mSymbol("Relu"))) {
      FuseSumRelu(pGraph, node, next(node));
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
  pA->output()->copyMetadata(pB->output());
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
  auto bn_inputs = pBNNode->inputs();
  auto scale_inputs = pScaleNode->inputs();
  // FIXME assume model is translated from caffe. so bn's scale is 1, bm's bias
  // is 0
  std::cout << "Warning: FuseBNScale has issue if model is not translated"
               "from caffe\n";
  const std::string &scale_name = scale_inputs[1]->uniqueName();
  const std::string &bias_name = scale_inputs[2]->uniqueName();
  const std::string &mean_name = bn_inputs[3]->uniqueName();
  const std::string &variance_name = bn_inputs[4]->uniqueName();

  std::vector<float> variance = getTensorData(pGraph, variance_name);
  std::vector<float> inv_var;
  const float epsilon = 1e-5f;
  float variance_average =
      std::accumulate(variance.begin(), variance.end(), 0) / variance.size();
  std::vector<bool> disable_channel(variance.size(), false);
  float threshold_zero = variance_average / 256;
  for (size_t i = 0; i < variance.size(); ++i) {
    inv_var.push_back(1.0 / sqrt(variance[i] + epsilon));
    if (variance[i] < threshold_zero) {
      disable_channel[i] = true;
    }
  }

  // calculate new scale and bias
  std::vector<float> new_scale;
  std::vector<float> new_bias;
  std::vector<float> scale_var = getTensorData(pGraph, scale_name);
  std::vector<float> bias_var = getTensorData(pGraph, bias_name);
  std::vector<float> mean_var = getTensorData(pGraph, mean_name);
  for (size_t i = 0; i < inv_var.size(); ++i) {
    if (!disable_channel[i]) {
      new_scale.push_back(inv_var[i] * scale_var[i]);
      new_bias.push_back(bias_var[i] -
                         (mean_var[i] * scale_var[i]) * inv_var[i]);
    } else {
      new_scale.push_back(0);
      new_bias.push_back(0);
    }
  }
  onnx::Tensor new_scale_tensor = *pGraph->getInitializer(scale_name);
  onnx::Tensor new_bias_tensor = *pGraph->getInitializer(bias_name);
  new_scale_tensor.set_raw_data((const char *)new_scale.data());
  new_bias_tensor.set_raw_data((const char *)new_bias.data());

  onnx::Value *new_scalar_value =
      pGraph->addInitializerAndInput(new_scale_tensor);
  onnx::Value *new_bias_value = pGraph->addInitializerAndInput(new_bias_tensor);

  // create Scale node
  onnx::Node *scale_node = pGraph->create(onnx::Symbol("Scale"));
  scale_node->addInput(pBNNode->inputs()[0]);
  scale_node->addInput(new_scalar_value);
  scale_node->addInput(new_bias_value);
  scale_node->output()->copyMetadata(pScaleNode->output());
  scale_node->copyAttributes(*pScaleNode);
  scale_node->insertBefore(pBNNode);
  pScaleNode->replaceAllUsesWith(scale_node);
  // remove unused initializer
  for (int i = 4; i >= 1; --i) {
    if (pBNNode->inputs()[i]->uses().size() == 1) {
      auto input = pBNNode->inputs()[i];
      pBNNode->removeInput(i);
      pGraph->eraseInitializerAndInput(input);
    }
  }
  for (int i = 2; i >= 1; --i) {
    if (pScaleNode->inputs()[i]->uses().size() == 1) {
      auto input = pScaleNode->inputs()[i];
      pScaleNode->removeInput(i);
      pGraph->eraseInitializerAndInput(input);
    }
  }
  pScaleNode->destroy();
  pBNNode->destroy();
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

void TGFuseOptimizer::FuseConvRelu(onnx::Graph *pGraph, onnx::Node *pConvNode,
                                   onnx::Node *pReluNode)
{
  Fuse(pConvNode, pReluNode)->i_(::onnx::Symbol("do_relu"), 1);
}

void TGFuseOptimizer::FuseSumRelu(onnx::Graph *pGraph, onnx::Node *pSumNode,
                                  onnx::Node *pReluNode)
{
  Fuse(pSumNode, pReluNode)->i_(::onnx::Symbol("do_relu"), 1);
}
