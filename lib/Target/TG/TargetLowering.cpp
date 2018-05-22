#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::CodeGenAndEmitInst(const ::onnx::Graph *pOnnxGraph)
{
  std::vector<std::unique_ptr<Operator> > &instList = m_pBackend->getInsts();
  instList.clear();
  for (auto it = pOnnxGraph->begin(), ie = pOnnxGraph->end(); it != ie; ++it) {
    const ::onnx::Node *const node = *it;
    LowerOperation(*node, instList);
  }
}
