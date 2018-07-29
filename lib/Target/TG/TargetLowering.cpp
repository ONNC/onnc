#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::ISelLowering(const ::onnx::Graph *pOnnxGraph,
                                  TGBackend::Instructions& pInsns)
{
  for (const ::onnx::Node *node : pOnnxGraph->nodes()) {
    std::unique_ptr<ComputeOperator2> oper(LowerOperation(*node, pInsns));
    if (oper != nullptr)
      pInsns.push_back(std::move(oper));
  }
}
