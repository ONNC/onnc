#include "BM168xISelLowering.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include "TGSoftmax.h"

#define DEBUG_TYPE "bm168x_lowering"
#include <onnc/Support/Debug.h>

using namespace onnc;

ComputeOperand2 *BM168xTargetLowering::LowerHelper(const ::onnx::Node &pNode)
{
  uint32_t symbol = pNode.kind();
  if (symbol == ::onnx::Symbol("Conv"))
    return new TGConv(pNode);
  else if (symbol == ::onnx::Symbol("Relu"))
    return new TGRelu(pNode);
  else if (symbol == ::onnx::Symbol("LRN"))
    return new TGLRN(pNode);
  else if (symbol == ::onnx::Symbol("MaxPool"))
    return new TGMaxPool(pNode);
  else if (symbol == ::onnx::Symbol("Gemm"))
    return new TGGemm(pNode);
  else if (symbol == ::onnx::Symbol("Softmax"))
    return new TGSoftmax(pNode);
  DEBUG(dbgs() << "unsupported node type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}

void BM168xTargetLowering::LowerOperation(
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
