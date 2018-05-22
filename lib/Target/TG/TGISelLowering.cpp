#include "TGConv.h"
#include "TGRelu.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGGemm.h"
#include "TGSoftmax.h"
#include "TGISelLowering.h"

#define DEBUG_TYPE "tg_lowering"
#include <onnc/Support/Debug.h>

using namespace onnc;

Operator*
TGTargetLowering::LowerHelper(const ::onnx::Node &node, MemTable &memTable)
{
  uint32_t symbol = node.kind();
  if (symbol == ::onnx::Symbol("Conv"))
    return new TGConv(node, memTable);
  else if (symbol == ::onnx::Symbol("Relu"))
    return new TGRelu(node, memTable);
  else if (symbol == ::onnx::Symbol("LRN"))
    return new TGLRN(node, memTable);
  else if (symbol == ::onnx::Symbol("MaxPool"))
    return new TGMaxPool(node, memTable);
  else if (symbol == ::onnx::Symbol("Gemm"))
    return new TGGemm(node, memTable);
  else if (symbol == ::onnx::Symbol("Softmax"))
    return new TGSoftmax(node, memTable);
  DEBUG(dbgs() << "unsupported node type: " << node.kind().toString()
                << std::endl;);
  return nullptr;
}

void TGTargetLowering::LowerOperation(
    const ::onnx::Node &node, std::vector<std::unique_ptr<Operator> > &instList)
{
  MemTable &globalMemLayout = m_pBackend->getMemLayout();
  std::unique_ptr<Operator> oper(LowerHelper(node, globalMemLayout));
  // FIXME ignore unsupported operation
  if (nullptr == oper)
    return;
  DEBUG(dbgs() << "lowering: " << oper->getName() << std::endl;);
  instList.push_back(std::move(oper));
}
