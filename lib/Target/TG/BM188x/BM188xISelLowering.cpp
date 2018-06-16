#define DEBUG_TYPE "bm188x_lowering"
#include "BM188xISelLowering.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include "TGSum.h"
#include <onnc/Support/Debug.h>

using namespace onnc;

ComputeOperator2 *BM188xISelLowering::LowerConv(
    const ::onnx::Node &pNode, ComputeGraph &pGraph,
    const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *bias = m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  auto *op = new BM188X::TGConv(pNode);
  op->update(&pLayerCtable);
  return op->addMemOperands(input, output, weight, bias);
}

ComputeOperator2 *BM188xISelLowering::LowerRelu(
    const ::onnx::Node &pNode, ComputeGraph &pGraph,
    const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGRelu(pNode);
  op->update(&pLayerCtable);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerMaxPool(
    const ::onnx::Node &pNode, ComputeGraph &pGraph,
    const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGMaxPool(pNode);
  op->update(&pLayerCtable);
  return op->addMemOperands(input, output);
}

ComputeOperator2 *BM188xISelLowering::LowerGemm(
    const ::onnx::Node &pNode, ComputeGraph &pGraph,
    const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
{
  auto *input = m_pBackend->getMemOperand(pNode.inputs()[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *weight = m_pBackend->getMemOperand(pNode.inputs()[1], MemType::WEIGHT);
  auto *bias = m_pBackend->getMemOperand(pNode.inputs()[2], MemType::WEIGHT);
  auto *op = new BM188X::TGGemm(pNode);
  op->update(&pLayerCtable);
  return op->addMemOperands(input, output, weight, bias);
}

ComputeOperator2 *BM188xISelLowering::LowerSum(
    const ::onnx::Node &pNode, ComputeGraph &pGraph,
    const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
{
  std::vector<MemOperand *> vInput;
  int input_size = pNode.inputs().size();

  for (int i = 0; i < input_size - 1; ++i)
    vInput.push_back(
        m_pBackend->getMemOperand(pNode.inputs()[i], MemType::NEURON));
  auto *output = m_pBackend->getMemOperand(pNode.outputs()[0], MemType::NEURON);
  auto *op = new BM188X::TGSum(pNode);
  op->update(&pLayerCtable);
  return op->addMemOperands(vInput, output);
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
  // Lowering without ctable
  if (symbol == ::onnx::Symbol("Undefined"))
    return nullptr;
  else if (symbol == ::onnx::Symbol("Reshape")) {
    return LowerReshape(pNode);
  }

  // Lowering with ctable
  std::string layerName =
      const_cast< ::onnx::Node &>(pNode).output()->uniqueName();
  const ::tg::bm1880::LayerCalibrationParameter &layerCtable =
      m_p1880backend->getCtableLayerParam(layerName);
  DEBUG(dbgs() << "layerName:" << layerName << "\n";);
  DEBUG(dbgs() << "LayerCalibrationParameter:" << layerCtable.DebugString(););
  if (symbol == ::onnx::Symbol("Conv")) {
    return LowerConv(pNode, pGraph, layerCtable);
  } else if (symbol == ::onnx::Symbol("Relu")) {
    return LowerRelu(pNode, pGraph, layerCtable);
  } else if (symbol == ::onnx::Symbol("MaxPool")) {
    return LowerMaxPool(pNode, pGraph, layerCtable);
  } else if (symbol == ::onnx::Symbol("Gemm")) {
    return LowerGemm(pNode, pGraph, layerCtable);
  } else if (symbol == ::onnx::Symbol("Sum")) {
    return LowerSum(pNode, pGraph, layerCtable);
  }
  DEBUG(dbgs() << "unsupported node type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}

void BM188xISelLowering::PrepareISelLowering(Module &pModule)
{
  // prepare ctable
  auto &ctable = m_pBackend->getCtable(pModule);
  m_pBackend->setCtableProto(ctable);
}
