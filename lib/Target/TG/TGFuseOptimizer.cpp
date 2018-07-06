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
  for (auto it = pGraph->rbegin(); it != pGraph->rend(); ++it) {
    auto *node = *it;
    if (match(node, mSymbol("Gemm")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
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
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("Sum")) and match(next(node), mSymbol("Relu"))) {
      FuseRelu(pGraph, node, next(node));
      return true;
    }
    if (match(node, mSymbol("BatchNormalization"))) {
      FuseBN(pGraph, node);
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

onnx::Node *TGFuseOptimizer::FuseChannelWiseMulAdd(onnx::Graph *pGraph,
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
  return scale_node;
}

static void replaceInput(onnx::Tensor &pTensor, size_t pIndex,
                         onnx::Node *pNode, onnx::Graph *pGraph)
{
  assert(pIndex < pNode->inputs().size());
  onnx::Value *new_value = pGraph->addInitializerAndInput(pTensor);
  onnx::Value *old_value = pNode->inputs()[pIndex];
  pNode->replaceInput(pIndex, new_value);
  if (old_value->uses().size() == 0) {
    pGraph->eraseInitializerAndInput(old_value);
  }
}

onnx::Node *TGFuseOptimizer::FuseBNScale(onnx::Graph *pGraph,
                                         onnx::Node *pBNNode,
                                         onnx::Node *pScaleNode)
{
  // We can fuse Scale into BN as follows:
  // Y = BN (X, scale, bias, mean, var)
  // Z = Scale (Y, mul, add)
  // into
  // Y = BN (X, new_scale, new_bias, mean, var)
  // with new scale = scale * mul,  new bias = bias * mul + add
  auto bn_inputs = pBNNode->inputs();
  auto scale_inputs = pScaleNode->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = scale_inputs[1]->uniqueName();
  const std::string &add_name = scale_inputs[2]->uniqueName();

  onnx::Tensor scale = *pGraph->getInitializer(scale_name);
  onnx::Tensor bias = *pGraph->getInitializer(bias_name);
  onnx::Tensor mul = *pGraph->getInitializer(mul_name);
  onnx::Tensor add = *pGraph->getInitializer(add_name);

  scale.multiply(mul);
  bias.multiply(mul);
  bias.add(add);

  replaceInput(scale, 1, pBNNode, pGraph);
  replaceInput(bias, 2, pBNNode, pGraph);

  for (int i = 2; i >= 1; --i) {
    if (scale_inputs[i]->uses().size() == 1) {
      auto input = scale_inputs[i];
      pScaleNode->removeInput(i);
      pGraph->eraseInitializerAndInput(input);
    }
  }
  pBNNode->output()->copyMetadata(pScaleNode->output());
  pScaleNode->replaceAllUsesWith(pBNNode);
  pScaleNode->destroy();
  return pBNNode;
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

onnx::Node *TGFuseOptimizer::FuseBN(onnx::Graph *pGraph, onnx::Node *pBNNode)
{
  // We can fuse the output computation as follows:
  //   ((x - mean) * (inv_var) * scale + bias
  // to
  //   (x * inv_var * scale) + (bias - mean * inv_var * scale)
  // as new Scale layer

  auto bn_inputs = pBNNode->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
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
  scale_node->i_(::onnx::Symbol("axis"), 1);
  scale_node->i_(::onnx::Symbol("broadcast"), 1);
  scale_node->output()->copyMetadata(pBNNode->output());
  scale_node->insertBefore(pBNNode);

  pBNNode->replaceAllUsesWith(scale_node);
  // remove unused initializer
  for (int i = 4; i >= 1; --i) {
    if (pBNNode->inputs()[i]->uses().size() == 1) {
      auto input = pBNNode->inputs()[i];
      pBNNode->removeInput(i);
      pGraph->eraseInitializerAndInput(input);
    }
  }
  pBNNode->destroy();
  return scale_node;
}
