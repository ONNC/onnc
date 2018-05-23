#include "BM188xISelLowering.h"
#include "TGConv.h"
#include "TGGemm.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGRelu.h"
#include "TGSoftmax.h"

#define DEBUG_TYPE "bm1880_lowering"
#include <onnc/Support/Debug.h>

using namespace onnc;

Operator *BM188xISelLowering::LowerHelper(const ::onnx::Node &pNode,
                                          MemTable &pMemTable)
{
  uint32_t symbol = pNode.kind();
  if (symbol == ::onnx::Symbol("Conv"))
    return new TGConv(pNode, pMemTable);
  else if (symbol == ::onnx::Symbol("Relu"))
    return new TGRelu(pNode, pMemTable);
  else if (symbol == ::onnx::Symbol("LRN"))
    return new TGLRN(pNode, pMemTable);
  else if (symbol == ::onnx::Symbol("MaxPool"))
    return new TGMaxPool(pNode, pMemTable);
  else if (symbol == ::onnx::Symbol("Gemm"))
    return new TGGemm(pNode, pMemTable);
  else if (symbol == ::onnx::Symbol("Softmax"))
    return new TGSoftmax(pNode, pMemTable);
  DEBUG(dbgs() << "unsupported pNode type: " << pNode.kind().toString()
               << std::endl;);
  return nullptr;
}

void BM188xISelLowering::LowerOperation(
    const ::onnx::Node &pNode,
    std::vector<std::unique_ptr<Operator> > &pInstList)
{
  MemTable &globalMemLayout = m_pBackend->getMemLayout();
  std::unique_ptr<Operator> oper(LowerHelper(pNode, globalMemLayout));
  // FIXME ignore unsupported operation
  if (nullptr == oper)
    return;
  DEBUG(dbgs() << "lowering: " << oper->getName() << std::endl;);
  pInstList.push_back(std::move(oper));
}

void BM188xISelLowering::PrepareCodeGenAndEmitInst(Module &pModule)
{
  // prepare ctable
  auto &ctable = m_pBackend->getCtable(pModule);
  DEBUG(dbgs() << "prepare ctable:" << ctable << "\n";);
}
