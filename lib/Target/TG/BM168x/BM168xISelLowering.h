#pragma once

#include "BM168xBackend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnx/common/ir.h>

namespace onnc {

class BM168xBackend;

class BM168xTargetLowering : public TargetLowering
{
public:
  BM168xTargetLowering(TGBackend *pBackend) : TargetLowering(pBackend) {}

  void LowerOperation(
      const ::onnx::Node &node,
      std::vector<std::unique_ptr<ComputeOperator2> > &instList) override;

private:
  ComputeOperator2 *LowerHelper(const ::onnx::Node &node);
};

} // namespace onnc
