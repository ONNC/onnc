#define DEBUG_TYPE "bm188x_lowering"
#include "BM188xISelLowering.h"
#include "TGAveragePool.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include "TGSum.h"
#include "TGUpsample.h"
#include <onnc/Support/Debug.h>

using namespace onnc;

ComputeOperator2 *BM188xISelLowering::LowerConv(const ::onnx::Node &pNode,
                                                ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *bias =
      (pNode.inputs().size() == 3)
          ? m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT)
          : nullptr;
  auto *op = new BM188X::TGConv(pNode);
  return op->addMemOperands(input, output, weight, bias);
}

ComputeOperator2 *BM188xISelLowering::LowerRelu(const ::onnx::Node &pNode,
                                                ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGRelu(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerMaxPool(const ::onnx::Node &pNode,
                                                   ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGMaxPool(pNode);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *
BM188xISelLowering::LowerAveragePool(const ::onnx::Node &pNode,
                                     ComputeGraph &pGraph)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGAveragePool(pNode);
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
  std::vector<MemOperand *> vInput;
  int input_size = pNode.inputs().size();

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

ComputeOperator2 *BM188xISelLowering::LowerReshape(const ::onnx::Node &pNode)
{
  // reshape is in-place layer
  // create MemOperand by input value with output name
  // reshape emit nop instruction
  m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON,
                            pNode.outputs()[0]->uniqueName());
  return nullptr;
}

ComputeOperator2 *BM188xISelLowering::LowerOperation(const ::onnx::Node &pNode,
                                                     ComputeGraph &pGraph)
{
  ::onnx::Node &node = const_cast< ::onnx::Node &>(pNode);
  DEBUG(dbgs() << "lowering node: name=" << node.name()
               << ", type=" << node.kind().toString() << "\n";);

  uint32_t symbol = pNode.kind();
  if (symbol == ::onnx::Symbol("Undefined"))
    return nullptr;
  else if (symbol == ::onnx::Symbol("Reshape")) {
    return LowerReshape(pNode);
  } else if (symbol == ::onnx::Symbol("Conv")) {
    return LowerConv(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Relu")) {
    return LowerRelu(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("MaxPool")) {
    return LowerMaxPool(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("AveragePool")) {
    return LowerAveragePool(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Gemm")) {
    return LowerGemm(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Sum")) {
    return LowerSum(pNode, pGraph);
  } else if (symbol == ::onnx::Symbol("Upsample")) {
    return LowerUpsample(pNode, pGraph);
  }
  DEBUG(dbgs() << "unsupported node type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}
