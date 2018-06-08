#pragma once

#include "BM188xBackend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnx/common/ir.h>

namespace onnc {

class BM1880Backend;

class BM188xISelLowering : public TargetLowering
{
public:
  BM188xISelLowering(TGBackend *pBackend) : TargetLowering(pBackend) {}

  void
  LowerOperation(const ::onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) override;

  void PrepareCodeGenAndEmitInst(Module &pModule) override;

private:
  Operator *LowerHelper(const ::onnx::Node &node);
};

} // namespace onnc
