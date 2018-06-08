#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::ISelLowering(const ::onnx::Graph *pOnnxGraph)
{
  std::vector<std::unique_ptr<ComputeOperand2> > &instList =
      m_pBackend->getInsts();
  instList.clear();
  for (const ::onnx::Node *node : pOnnxGraph->nodes()) {
    LowerOperation(*node, instList);
  }
}
