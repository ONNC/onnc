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
  BM188xISelLowering(TGBackend *pBackend) : TargetLowering(pBackend)
  {
    m_p1880backend = static_cast<BM1880Backend *>(pBackend);
  }

  void LowerOperation(
      const ::onnx::Node &node,
      std::vector<std::unique_ptr<ComputeOperator2> > &instList) override;

  void PrepareISelLowering(Module &pModule) override;

private:
  ComputeOperator2 *LowerHelper(const ::onnx::Node &node);
  BM1880Backend *m_p1880backend;
};

} // namespace onnc
