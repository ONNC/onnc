#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::ISelLowering(const ::onnx::Graph *pOnnxGraph)
{
  std::vector<std::unique_ptr<ComputeOperator2> > &instList =
      m_pBackend->getInsts();
  instList.clear();
  for (const ::onnx::Node *node : pOnnxGraph->nodes()) {
    std::unique_ptr<ComputeOperator2> oper(LowerOperation(*node, instList));
    if (oper != nullptr)
      instList.push_back(std::move(oper));
  }
}
