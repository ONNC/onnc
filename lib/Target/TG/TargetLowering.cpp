#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::CodeGenAndEmitInst(const ::onnx::Graph *pOnnxGraph)
{
  std::vector<std::unique_ptr<Operator> > &instList = m_pBackend->getInsts();
  instList.clear();
  for (const ::onnx::Node *node : pOnnxGraph->nodes()) {
    LowerOperation(*node, instList);
  }
}
