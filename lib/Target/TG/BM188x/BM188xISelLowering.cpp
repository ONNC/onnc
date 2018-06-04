#define DEBUG_TYPE "bm188x_lowering"
#include "BM188xISelLowering.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include <onnc/Support/Debug.h>

using namespace onnc;

ComputeOperand2 *BM188xISelLowering::LowerHelper(const ::onnx::Node &pNode)
{
  uint32_t symbol = pNode.kind();
  ::onnx::ArrayRef<const ::onnx::Value *> inputs = pNode.inputs();
  ::onnx::ArrayRef<const ::onnx::Value *> outputs = pNode.outputs();

  if (symbol == ::onnx::Symbol("Undefined"))
    return nullptr;
  else if (symbol == ::onnx::Symbol("Reshape")) {
    // reshape is in-place layer
    // create MemOperand by input value with output name
    // reshape emit nop instruction
    m_pBackend->getMemOperand(inputs[0], MemType::NEURON);
    m_pBackend->getMemOperand(inputs[0], MemType::NEURON,
                              outputs[0]->uniqueName());
    return nullptr;
  }

  std::string layerName =
      const_cast< ::onnx::Node &>(pNode).output()->uniqueName();
  const LayerCalibrationParameter &layerCtable =
      m_p1880backend->getCtableLayerParam(layerName);
  DEBUG(dbgs() << "layerName:" << layerName << "\n";);
  DEBUG(dbgs() << "LayerCalibrationParameter:" << layerCtable.DebugString(););

  auto *input = m_pBackend->getMemOperand(inputs[0], MemType::NEURON);
  auto *output = m_pBackend->getMemOperand(outputs[0], MemType::NEURON);
  if (symbol == ::onnx::Symbol("Conv")) {
    ComputeOperand2 *op = new BM188X::TGConv(pNode, layerCtable);
    auto *weight = m_pBackend->getMemOperand(inputs[1], MemType::WEIGHT);
    auto *bias = m_pBackend->getMemOperand(inputs[2], MemType::WEIGHT);
    return op->addMemOperand(input)
        ->addMemOperand(weight)
        ->addMemOperand(output)
        ->addMemOperand(bias);
  } else if (symbol == ::onnx::Symbol("Relu")) {
    ComputeOperand2 *op = new BM188X::TGRelu(pNode, layerCtable);
    return op->addMemOperand(input)->addMemOperand(output);
  } else if (symbol == ::onnx::Symbol("LRN")) {
    ComputeOperand2 *op = new BM188X::TGLRN(pNode, layerCtable);
    return op->addMemOperand(input)->addMemOperand(output);
  } else if (symbol == ::onnx::Symbol("MaxPool")) {
    ComputeOperand2 *op = new BM188X::TGMaxPool(pNode, layerCtable);
    return op->addMemOperand(input)->addMemOperand(output);
  } else if (symbol == ::onnx::Symbol("Gemm")) {
    ComputeOperand2 *op = new BM188X::TGGemm(pNode, layerCtable);
    auto *weight = m_pBackend->getMemOperand(inputs[1], MemType::WEIGHT);
    auto *bias = m_pBackend->getMemOperand(inputs[2], MemType::WEIGHT);
    return op->addMemOperand(input)
        ->addMemOperand(weight)
        ->addMemOperand(bias)
        ->addMemOperand(output);
  }
  DEBUG(dbgs() << "unsupported node type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}

void BM188xISelLowering::LowerOperation(
    const ::onnx::Node &pNode,
    std::vector<std::unique_ptr<ComputeOperand2> > &pInstList)
{
  std::unique_ptr<ComputeOperand2> oper(LowerHelper(pNode));
  // FIXME ignore unsupported operation
  if (nullptr == oper)
    return;
  DEBUG(dbgs() << "lowering type: " << oper->getTypeName()
               << "\nlayer name:" << oper->getLayerName() << "\n";);
  pInstList.push_back(std::move(oper));
}

void BM188xISelLowering::PrepareISelLowering(Module &pModule)
{
  // prepare ctable
  auto &ctable = m_pBackend->getCtable(pModule);
  m_pBackend->setCtableProto(ctable);
}
