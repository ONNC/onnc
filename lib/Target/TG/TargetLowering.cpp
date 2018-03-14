#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::CodeGenAndEmitInst(
    onnx::Graph *onnxGraph, std::vector<std::unique_ptr<Operator> > &instList) {
  instList.clear();
  for (auto it = onnxGraph->begin(), ie = onnxGraph->end(); it != ie;
       ++it) {
    const onnx::Node *const node = *it;
    LowerOperation(*node, instList);
  }
}
