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
#define DEBUG_TYPE "bm188x_lowering"
#include "BM188xISelLowering.h"
#include "TGAveragePool.h"
#include "TGConcat.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGGlobalAveragePool.h"
#include "TGLRN.h"
#include "TGLeakyRelu.h"
#include "TGMaxPool.h"
#include "TGPRelu.h"
#include "TGRelu.h"
#include "TGScale.h"
#include "TGSum.h"
#include "TGTranspose.h"
#include "TGUpsample.h"
#include "TLConv.h"
#include "TLLoad.h"
#include "TLPool.h"
#include "TLStore.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TargetTransformInfo.h>

using namespace onnc;

ComputeOperator2 *BM188xISelLowering::LowerConv(const ::onnx::Node &pNode,
                                                ComputeGraph &pGraph)
{
  if (pNode.hasAttribute(::onnx::Symbol("is_sliced"))) {
    auto is_sliced = pNode.i(::onnx::Symbol("is_sliced"));
    if (is_sliced)
      return LowerTLConv(pNode, pGraph);
  }

  auto *op = new BM188X::TGConv(pNode);
  auto *input_memop =
      m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output_memop =
      m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight_memop =
      m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  MemOperand *bias_memop = nullptr;
  MemOperand *scale_memop = nullptr;
  MemOperand *scale_bias_memop = nullptr;
  bool do_bias = op->isDoBias();
  bool do_scale = op->isDoScale();
  bool do_scale_bias = op->isDoScaleBias();

  int idx = 2;
  if (do_bias)
    bias_memop =
        m_pBackend->getMemOperand(pNode.inputs()[idx++], MemType::WEIGHT);
  if (do_scale)
    scale_memop =
        m_pBackend->getMemOperand(pNode.inputs()[idx++], MemType::WEIGHT);
  if (do_scale_bias)
    scale_bias_memop =
        m_pBackend->getMemOperand(pNode.inputs()[idx++], MemType::WEIGHT);
  return op->addMemOperands(input_memop, output_memop, weight_memop, bias_memop,
                            scale_memop, scale_bias_memop);
}

ComputeOperator2 *BM188xISelLowering::LowerTLConv(const ::onnx::Node &pNode,
                                                  ComputeGraph &pGraph)
{
  auto *op = new BM188X::TLConv(pNode);
  auto *input_memop =
      m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output_memop =
      m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight_memop =
      m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  bool do_bias = op->isDoBias();
  MemOperand *bias_memop = nullptr;
  if (do_bias)
    bias_memop = m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  op->addMemOperands(input_memop, weight_memop, output_memop, bias_memop);
  return op;
}

ComputeOperator2 *BM188xISelLowering::LowerTLLoad(const ::onnx::Node &pNode,
                                                  ComputeGraph &pGraph)
{
  auto *op = new BM188X::TLLoad(pNode);
  auto is_neuron = pNode.i(::onnx::Symbol("is_neuron"));
  MemType mem_type;
  if (is_neuron) {
    mem_type = MemType::NEURON;
  } else {
    mem_type = MemType::WEIGHT;
  }
  auto *input_memop = m_pBackend->getMemOperand(pNode.inputs()[0], mem_type);
  op->addMemOperands(input_memop);
  return op;
}

ComputeOperator2 *BM188xISelLowering::LowerTLStore(const ::onnx::Node &pNode,
                                                   ComputeGraph &pGraph)
{
  auto *op = new BM188X::TLStore(pNode);
  auto is_neuron = pNode.i(::onnx::Symbol("is_neuron"));
  MemType mem_type;
  if (is_neuron) {
    mem_type = MemType::NEURON;
  } else {
    mem_type = MemType::WEIGHT;
  }
  // FIXME(arcbbb): It's a workaround.
  // not to violate SSA, we add output value as input.
  auto *output_memop = m_pBackend->getMemOperand(pNode.inputs()[0], mem_type);
  op->addMemOperands(output_memop);
  return op;
}

ComputeOperator2 *BM188xISelLowering::LowerRelu(const ::onnx::Node &pNode,
                                                ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGRelu(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerPRelu(const ::onnx::Node &pNode,
                                                 ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *slope = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);

  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);

  auto *op = new BM188X::TGPRelu(pNode);

  return op->addMemOperands(input, slope, output);
}

ComputeOperator2 *BM188xISelLowering::LowerLeakyRelu(const ::onnx::Node &pNode,
                                                     ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);

  auto *op = new BM188X::TGLeakyRelu(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerTLPool(const ::onnx::Node &pNode,
                                                  ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TLPool(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerMaxPool(const ::onnx::Node &pNode,
                                                   ComputeGraph &pGraph)
{
  if (pNode.hasAttribute(::onnx::Symbol("is_sliced"))) {
    auto is_sliced = pNode.i(::onnx::Symbol("is_sliced"));
    if (is_sliced)
      return LowerTLPool(pNode, pGraph);
  }
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGMaxPool(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *
BM188xISelLowering::LowerAveragePool(const ::onnx::Node &pNode,
                                     ComputeGraph &pGraph)
{
  if (pNode.hasAttribute(::onnx::Symbol("is_sliced"))) {
    auto is_sliced = pNode.i(::onnx::Symbol("is_sliced"));
    if (is_sliced)
      return LowerTLPool(pNode, pGraph);
  }
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGAveragePool(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *
BM188xISelLowering::LowerGlobalAveragePool(const ::onnx::Node &pNode,
                                           ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGGlobalAveragePool(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerGemm(const ::onnx::Node &pNode,
                                                ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *bias = m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  auto *op = new BM188X::TGGemm(pNode);
  return op->addMemOperands(input, output, weight, bias);
}

ComputeOperator2 *BM188xISelLowering::LowerSum(const ::onnx::Node &pNode,
                                               ComputeGraph &pGraph)
{
  // BM188x does not support NEURON addition with Numpy-style broadcasting
  // TODO refactoring checking
  auto dim_input0 = pNode.inputs()[0]->sizes();
  int input_size = pNode.inputs().size();
  for (int i = 1; i < input_size; ++i) {
    auto dim_input_i = pNode.inputs()[i]->sizes();
    assert(dim_input0.size() == dim_input_i.size());
    for (size_t j = 0; j < dim_input0.size(); ++j) {
      assert(dim_input0[j].dim == dim_input_i[j].dim);
    }
  }

  std::vector<MemOperand *> vInput;

  for (int i = 0; i < input_size; ++i) {
    vInput.push_back(
        m_pBackend->getMemOperand(pNode.inputs()[i], MemType::NEURON));
  }
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGSum(pNode);
  return op->addMemOperands(vInput, output);
}

ComputeOperator2 *BM188xISelLowering::LowerUpsample(const ::onnx::Node &pNode,
                                                    ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGUpsample(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerLRN(const onnx::Node &pNode,
                                               ComputeGraph &pGraph)
{
  auto &node = const_cast<onnx::Node &>(pNode);
  auto *graph = node.owningGraph();
  auto output_name = pNode.outputs()[0]->uniqueName();
  int npu_num = m_p1880backend->getTTI()->getWarpSize();
  // add SQR LUT table
  onnx::Tensor sqrlut_tensor;
  sqrlut_tensor.sizes().push_back(256 * npu_num);
  sqrlut_tensor.elem_type() = onnx::TensorProto_DataType_INT8;
  std::string squlut_name = output_name + "_sqrlut";
  auto *sqrlut_val = graph->addInitializerAndInput(sqrlut_tensor, squlut_name);
  node.addInput(sqrlut_val);

  // add POWER LUT table
  onnx::Tensor powerlut_tensor;
  powerlut_tensor.sizes().push_back(256 * npu_num);
  powerlut_tensor.elem_type() = onnx::TensorProto_DataType_INT8;
  std::string powerlut_name = output_name + "_powerlut";
  auto *powerlut_val =
      graph->addInitializerAndInput(powerlut_tensor, powerlut_name);
  node.addInput(powerlut_val);

  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *sqrlut = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *powerlut =
      m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGLRN(pNode);
  return op->addMemOperands(input, sqrlut, powerlut, output);
}

ComputeOperator2 *BM188xISelLowering::Lower2NopInst(const ::onnx::Node &pNode)
{
  // emit nop instruction
  // nop is in-place layer
  // create MemOperand by input value with output name
  m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON,
                            pNode.outputs()[0]->uniqueName());
  return nullptr;
}

ComputeOperator2 *BM188xISelLowering::LowerConcat(const ::onnx::Node &pNode,
                                                  ComputeGraph &pGraph)
{
  std::vector<MemOperand *> input;
  for (size_t i = 0; i < pNode.inputs().size(); i++) {
    input.push_back(
        m_pBackend->getMemOperand(pNode.inputs()[i], MemType::NEURON));
  }
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGConcat(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerTranspose(const ::onnx::Node &pNode,
                                                     ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGTranspose(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerScale(const ::onnx::Node &pNode,
                                                 ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *scale = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *bias = m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGScale(pNode);
  return op->addMemOperands(input, scale, bias, output);
}

ComputeOperator2 *BM188xISelLowering::LowerOperation(const ::onnx::Node &pNode,
                                                     ComputeGraph &pGraph)
{
  ::onnx::Node &node = const_cast< ::onnx::Node &>(pNode);
  DEBUG(dbgs() << "lowering node: name=" << node.name()
               << ", type=" << node.kind().toString() << "\n";);

  uint32_t symbol = pNode.kind();
  if (symbol == ::onnx::Symbol("Undefined")) {
    return nullptr;
  } else if (symbol == ::onnx::Symbol("Reshape") ||
             symbol == ::onnx::Symbol("Flatten")) {
    return Lower2NopInst(pNode);
  } else if (symbol == ::onnx::Symbol("Concat")) {
    return LowerConcat(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Conv")) {
    return LowerConv(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Relu")) {
    return LowerRelu(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("PRelu")) {
    return LowerPRelu(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("LeakyRelu")) {
    return LowerLeakyRelu(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("MaxPool")) {
    return LowerMaxPool(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("AveragePool")) {
    return LowerAveragePool(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("GlobalAveragePool")) {
    return LowerGlobalAveragePool(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Gemm")) {
    return LowerGemm(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Sum")) {
    return LowerSum(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Upsample")) {
    return LowerUpsample(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Transpose")) {
    return LowerTranspose(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("TLLoad")) {
    return LowerTLLoad(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("TLStore")) {
    return LowerTLStore(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("LRN")) {
    return LowerLRN(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Scale")) {
    return LowerScale(pNode, pGraph);
  }
  std::cout << "Warning: unsupported node type: " << pNode.kind().toString()
            << "\n";
  return nullptr;
}
