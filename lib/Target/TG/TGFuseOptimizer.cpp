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

bool TGFuseOptimizer::FuseOpset6Nodes(onnx::Graph *pGraph, onnx::Node *pNode)
{
  if (match(pNode, mSymbol("BatchNormalization")) and
      match(next(pNode), mSymbol("Mul"), mAttr("axis", 1),
            mTrueAttr("broadcast"))) {
    FuseBNMulV6(pGraph, pNode, next(pNode));
    return true;
  }
  if (match(pNode, mSymbol("BatchNormalization")) and
      match(next(pNode), mSymbol("Add"), mAttr("axis", 1),
            mTrueAttr("broadcast"))) {
    FuseBNAddV6(pGraph, pNode, next(pNode));
    return true;
  }
  return false;
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

// We can fuse Mul into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// Z = Mul (Y, mul)
// into
// Y = BN (X, new_scale, new_bias, mean, var)
// with new scale = scale * mul, new bias = bias * mul
onnx::Node *TGFuseOptimizer::FuseBNMulV6(onnx::Graph *pGraph,
                                         onnx::Node *pBNNode,
                                         onnx::Node *pMulNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto mul_inputs = pMulNode->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = mul_inputs[1]->uniqueName();

  onnx::Tensor scale = *pGraph->getInitializer(scale_name);
  onnx::Tensor bias = *pGraph->getInitializer(bias_name);
  onnx::Tensor mul = *pGraph->getInitializer(mul_name);

  scale.multiply(mul);
  bias.multiply(mul);
  replaceInput(scale, 1, pBNNode, pGraph);
  replaceInput(bias, 2, pBNNode, pGraph);

  if (mul_inputs[1]->uses().size() == 1) {
    auto input = mul_inputs[1];
    pMulNode->removeInput(1);
    pGraph->eraseInitializerAndInput(input);
  }

  pBNNode->output()->copyMetadata(pMulNode->output());
  pMulNode->replaceAllUsesWith(pBNNode);
  pMulNode->destroy();
  return pBNNode;
}

// We can fuse Add into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// Z = Add (Y, add)
// into
// Y = BN (X, scale, new_bias, mean, var)
// with new bias = bias + add
onnx::Node *TGFuseOptimizer::FuseBNAddV6(onnx::Graph *pGraph,
                                         onnx::Node *pBNNode,
                                         onnx::Node *pAddNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto add_inputs = pAddNode->inputs();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &add_name = add_inputs[1]->uniqueName();

  onnx::Tensor bias = *pGraph->getInitializer(bias_name);
  onnx::Tensor add = *pGraph->getInitializer(add_name);

  bias.add(add);
  replaceInput(bias, 2, pBNNode, pGraph);

  if (add_inputs[1]->uses().size() == 1) {
    auto input = add_inputs[1];
    pAddNode->removeInput(1);
    pGraph->eraseInitializerAndInput(input);
  }

  pBNNode->output()->copyMetadata(pAddNode->output());
  pAddNode->replaceAllUsesWith(pBNNode);
  pAddNode->destroy();
  return pBNNode;
}

// We can fuse Mul into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// mul = unsqueeze (mul_tensor)
// Z = Mul (Y, mul)
// into
// Y = BN (X, new_scale, new_bias, mean, var)
// with new scale = scale * mul_tensor, new bias = bias * mul_tensor
onnx::Node *TGFuseOptimizer::FuseBNMul(onnx::Graph *pGraph, onnx::Node *pBNNode,
                                       onnx::Node *pMulNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto mul_inputs = pMulNode->inputs();
  auto *unsque_node = mul_inputs[1]->node();
  auto unsque_inputs = unsque_node->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = unsque_inputs[0]->uniqueName();

  onnx::Tensor scale = *pGraph->getInitializer(scale_name);
  onnx::Tensor bias = *pGraph->getInitializer(bias_name);
  onnx::Tensor mul = *pGraph->getInitializer(mul_name);
  mul.sizes() = scale.sizes();

  scale.multiply(mul);
  bias.multiply(mul);
  replaceInput(scale, 1, pBNNode, pGraph);
  replaceInput(bias, 2, pBNNode, pGraph);

  if (unsque_inputs[0]->uses().size() == 1) {
    auto input = unsque_inputs[0];
    unsque_node->removeInput(0);
    pGraph->eraseInitializerAndInput(input);
  }

  pBNNode->output()->copyMetadata(pMulNode->output());
  pMulNode->replaceAllUsesWith(pBNNode);
  pMulNode->destroy();
  if (unsque_node->output()->uses().size() == 0) {
    unsque_node->destroy();
  }
  return pBNNode;
}

// We can fuse Add into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// add = unsqueeze (add_tensor)
// Z = Add (Y, add)
// into
// Y = BN (X, scale, new_bias, mean, var)
// with new bias = bias + add_tensor
onnx::Node *TGFuseOptimizer::FuseBNAdd(onnx::Graph *pGraph, onnx::Node *pBNNode,
                                       onnx::Node *pAddNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto add_inputs = pAddNode->inputs();
  auto *unsque_node = add_inputs[1]->node();
  auto unsque_inputs = unsque_node->inputs();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &add_name = unsque_inputs[0]->uniqueName();

  onnx::Tensor bias = *pGraph->getInitializer(bias_name);
  onnx::Tensor add = *pGraph->getInitializer(add_name);
  add.sizes() = bias.sizes();

  bias.add(add);
  replaceInput(bias, 2, pBNNode, pGraph);

  if (unsque_inputs[0]->uses().size() == 1) {
    auto input = unsque_inputs[0];
    unsque_node->removeInput(0);
    pGraph->eraseInitializerAndInput(input);
  }

  pBNNode->output()->copyMetadata(pAddNode->output());
  pAddNode->replaceAllUsesWith(pBNNode);
  pAddNode->destroy();
  if (unsque_node->output()->uses().size() == 0) {
    unsque_node->destroy();
  }
  return pBNNode;
}

bool TGFuseOptimizer::FuseOpset7Nodes(onnx::Graph *pGraph, onnx::Node *pNode)
{
  if (match(pNode, mSymbol("BatchNormalization")) &&
      match(next(pNode), mSymbol("Mul")) &&
      match(input(next(pNode), 1), mSymbol("Unsqueeze"))) {
    auto unsq_dims = next(pNode)->inputs()[1]->sizes();
    if (unsq_dims.size() != 3 || unsq_dims[1].dim != 1 ||
        unsq_dims[2].dim != 1) {
      return false;
    }
    // shape(unsqueeze) = (C, 1, 1)
    FuseBNMul(pGraph, pNode, next(pNode));
    return true;
  }
  if (match(pNode, mSymbol("BatchNormalization")) &&
      match(next(pNode), mSymbol("Add")) &&
      match(input(next(pNode), 1), mSymbol("Unsqueeze"))) {
    auto unsq_dims = next(pNode)->inputs()[1]->sizes();
    if (unsq_dims.size() != 3 || unsq_dims[1].dim != 1 ||
        unsq_dims[2].dim != 1) {
      return false;
    }
    // shape(unsqueeze) = (C, 1, 1)
    FuseBNAdd(pGraph, pNode, next(pNode));
    return true;
  }
  return false;
}

bool TGFuseOptimizer::FuseNodes(onnx::Graph *pGraph,
                                const int64_t &pOpsetVersion)
{
  for (auto it = pGraph->rbegin(); it != pGraph->rend(); ++it) {
    auto *node = *it;
    if (6 == pOpsetVersion && FuseOpset6Nodes(pGraph, node)) {
      return true;
    }
    if (6 < pOpsetVersion && FuseOpset7Nodes(pGraph, node)) {
      return true;
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
    if (match(node, mSymbol("BatchNormalization"))) {
      FuseBN(pGraph, node);
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
  bool is_changed = false, local_changed;
  do {
    local_changed = FuseNodes(pGraph, pOpsetVersion);
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
