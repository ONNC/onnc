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

static std::vector<float> getTensorData(xGraph *pGraph,
                                        const std::string &pName)
{
  const xTensor &tensor = *pGraph->getInitializer(pName);
  assert(tensor.elem_type() == (xTensorProtoDataType)xValueType::kFloat);
  if (tensor.is_raw_data()) {
    size_t count = onnc::getTotalCount(tensor.sizes());
    const std::string &raw = tensor.raw();
    std::vector<float> data(count);
    std::memcpy(data.data(), raw.data(), count * sizeof(float));
    return data;
  }
  return tensor.floats();
}

static inline bool isTensor(xValue *pValue)
{
  xGraph *graph = pValue->owningGraph();
  std::unordered_set<std::string> init_names(graph->initializer_names().begin(),
                                             graph->initializer_names().end());
  if (init_names.count(pValue->uniqueName()))
    return true;
  return false;
}

static bool isNeuronInputs(xNode *pAddNode)
{
  xGraph *graph = pAddNode->owningGraph();
  std::unordered_set<std::string> init_names(graph->initializer_names().begin(),
                                             graph->initializer_names().end());
  auto input0 = pAddNode->inputs()[0]->uniqueName();
  auto input1 = pAddNode->inputs()[1]->uniqueName();
  if (init_names.count(input0) || init_names.count(input1))
    return false;
  return true;
}

bool TGFuseOptimizer::FuseOpset6Nodes(xGraph *pGraph, xNode *pNode)
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

static void replaceInput(xTensor &pTensor, size_t pIndex,
                         xNode *pNode, xGraph *pGraph)
{
  assert(pIndex < pNode->inputs().size());
  xValue *new_value = pGraph->addInitializerAndInput(pTensor);
  xValue *old_value = pNode->inputs()[pIndex];
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
xNode *
TGFuseOptimizer::FuseBNMulV6(xGraph *pGraph, xNode *pBNNode, xNode *pMulNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto mul_inputs = pMulNode->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = mul_inputs[1]->uniqueName();

  xTensor scale = *pGraph->getInitializer(scale_name);
  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor mul = *pGraph->getInitializer(mul_name);

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
xNode *TGFuseOptimizer::FuseBNAddV6(xGraph *pGraph,
                                    xNode *pBNNode,
                                    xNode *pAddNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto add_inputs = pAddNode->inputs();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &add_name = add_inputs[1]->uniqueName();

  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor add = *pGraph->getInitializer(add_name);

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
// Z = Mul (Y, mul_tensor)
// into
// Y = BN (X, new_scale, new_bias, mean, var)
// with new scale = scale * mul_tensor_tensor, new bias = bias *
// mul_tensor_tensor
xNode *TGFuseOptimizer::FuseBNMulTensor(xGraph *pGraph,
                                        xNode *pBNNode,
                                        xNode *pMulNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto mul_inputs = pMulNode->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = mul_inputs[1]->uniqueName();

  xTensor scale = *pGraph->getInitializer(scale_name);
  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor mul = *pGraph->getInitializer(mul_name);
  mul.sizes() = scale.sizes();

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

// We can fuse Mul into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// mul = unsqueeze (mul_tensor)
// Z = Mul (Y, mul)
// into
// Y = BN (X, new_scale, new_bias, mean, var)
// with new scale = scale * mul_tensor, new bias = bias * mul_tensor
xNode *
TGFuseOptimizer::FuseBNMul(xGraph *pGraph, xNode *pBNNode, xNode *pMulNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto mul_inputs = pMulNode->inputs();
  auto *unsque_node = mul_inputs[1]->node();
  auto unsque_inputs = unsque_node->inputs();
  const std::string &scale_name = bn_inputs[1]->uniqueName();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &mul_name = unsque_inputs[0]->uniqueName();

  xTensor scale = *pGraph->getInitializer(scale_name);
  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor mul = *pGraph->getInitializer(mul_name);
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
// Z = Add (Y, add_tensor)
// into
// Y = BN (X, scale, new_bias, mean, var)
// with new bias = bias + add_tensor
xNode *TGFuseOptimizer::FuseBNAddTensor(xGraph *pGraph,
                                        xNode *pBNNode,
                                        xNode *pAddNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto add_inputs = pAddNode->inputs();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &add_name = add_inputs[1]->uniqueName();

  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor add = *pGraph->getInitializer(add_name);
  add.sizes() = bias.sizes();

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

// We can fuse Add into BN as follows:
// Y = BN (X, scale, bias, mean, var)
// add = unsqueeze (add_tensor)
// Z = Add (Y, add)
// into
// Y = BN (X, scale, new_bias, mean, var)
// with new bias = bias + add_tensor
xNode *TGFuseOptimizer::FuseBNAdd(xGraph *pGraph, xNode *pBNNode,
                                       xNode *pAddNode)
{
  auto bn_inputs = pBNNode->inputs();
  auto add_inputs = pAddNode->inputs();
  auto *unsque_node = add_inputs[1]->node();
  auto unsque_inputs = unsque_node->inputs();
  const std::string &bias_name = bn_inputs[2]->uniqueName();
  const std::string &add_name = unsque_inputs[0]->uniqueName();

  xTensor bias = *pGraph->getInitializer(bias_name);
  xTensor add = *pGraph->getInitializer(add_name);
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

bool TGFuseOptimizer::FuseOpset7Nodes(xGraph *pGraph, xNode *pNode)
{
  if (match(pNode, mSymbol("BatchNormalization")) &&
      match(next(pNode), mSymbol("Mul")) &&
      isTensor(next(pNode)->inputs()[1])) {
    auto tensor_dims = next(pNode)->inputs()[1]->sizes();
    if (tensor_dims.size() != 3 || tensor_dims[1].dim != 1 ||
        tensor_dims[2].dim != 1) {
      return false;
    }
    // tensor =  (C, 1, 1)
    FuseBNMulTensor(pGraph, pNode, next(pNode));
    return true;
  }

  if (match(pNode, mSymbol("BatchNormalization")) &&
      match(next(pNode), mSymbol("Add")) &&
      isTensor(next(pNode)->inputs()[1])) {
    auto tensor_dims = next(pNode)->inputs()[1]->sizes();
    if (tensor_dims.size() != 3 || tensor_dims[1].dim != 1 ||
        tensor_dims[2].dim != 1) {
      return false;
    }
    // tensor =  (C, 1, 1)
    FuseBNMulTensor(pGraph, pNode, next(pNode));
    return true;
  }

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

bool TGFuseOptimizer::FuseNodes(xGraph *pGraph,
                                const int64_t &pOpsetVersion)
{
  // WARNING: change iteration order will impact the optimization result
  for (auto it = pGraph->begin(); it != pGraph->end(); ++it) {
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
    if (match(node, mSymbol("Add")) && isNeuronInputs(node)) {
      AliasSumOperator(pGraph, node);
      return true;
    }
    if (match(node, mSymbol("Conv")) and match(next(node), mSymbol("Scale"))) {
      FuseConvScale(pGraph, node, next(node));
      return true;
    }
  }
  return false;
}

bool TGFuseOptimizer::FuseOptimization(xGraph *pGraph,
                                       const int64_t &pOpsetVersion)
{
  bool is_changed = false, local_changed;
  do {
    local_changed = FuseNodes(pGraph, pOpsetVersion);
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
  pConvNode->addInput(pScaleNode->inputs()[1]);
  pConvNode->addInput(pScaleNode->inputs()[2]);
  pConvNode->i_(xSymbol("do_scale"), 1)->i_(xSymbol("do_scale_bias"), 1);
  pConvNode->output()->copyMetadata(pScaleNode->output());
  pScaleNode->replaceAllUsesWith(pConvNode);
  pScaleNode->destroy();
  return pConvNode;
}

xNode *
TGFuseOptimizer::FuseRelu(xGraph *pGraph, xNode *pNode, xNode *pReluNode)
{
  Fuse(pNode, pReluNode)->i_(xSymbol("do_relu"), 1);
  return pNode;
}

xNode *TGFuseOptimizer::FuseBN(xGraph *pGraph, xNode *pBNNode)
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
      std::accumulate(variance.begin(), variance.end(), 0.0) / variance.size();
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

  xTensor new_scale_tensor = *pGraph->getInitializer(scale_name);
  if (new_scale_tensor.is_raw_data()) {
    new_scale_tensor.set_raw_data((const char *)new_scale.data());
  } else {
    new_scale_tensor.floats() = new_scale;
  }

  xTensor new_bias_tensor = *pGraph->getInitializer(bias_name);
  if (new_bias_tensor.is_raw_data()) {
    new_bias_tensor.set_raw_data((const char *)new_bias.data());
  } else {
    new_bias_tensor.floats() = new_bias;
  }

  xValue *new_scalar_value =
      pGraph->addInitializerAndInput(new_scale_tensor);
  xValue *new_bias_value = pGraph->addInitializerAndInput(new_bias_tensor);

  // create Scale node
  xNode *scale_node = pGraph->create(xSymbol("Scale"));
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

xNode *TGFuseOptimizer::AliasSumOperator(xGraph *pGraph,
                                              xNode *pAddNode)
{
  xNode *sum_node = pGraph->create(xSymbol("Sum"));
  sum_node->addInput(pAddNode->inputs()[0]);
  sum_node->addInput(pAddNode->inputs()[1]);
  sum_node->output()->copyMetadata(pAddNode->output());
  sum_node->insertBefore(pAddNode);
  pAddNode->replaceAllUsesWith(sum_node);
  pAddNode->destroy();
  return sum_node;
}
