#pragma once

#include <onnx/common/ir.h>
#include "Operator.h"
#include <memory>

namespace onnc {

class TargetLowering
{
public:
  TargetLowering() {}

  ~TargetLowering() {}

  void CodeGenAndEmitInst(::onnx::Graph *graph,
                          std::vector<std::unique_ptr<Operator> > &instList);

  virtual void
  LowerOperation(const ::onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) = 0;
};

} // namespace onnc
