#pragma once

#include "ComputeOperand.h"
#include "TGBackend.h"
#include <memory>
#include <onnc/IR/Module.h>
#include <onnx/common/ir.h>

namespace onnc {

class Module;
class TGBackend;

class TargetLowering
{
public:
  TargetLowering(TGBackend *pBackend) : m_pBackend(pBackend) {}

  virtual ~TargetLowering() = default;

  // do something before ISelLowering
  virtual void PrepareISelLowering(Module &pModule) { return; };

  // Lowering ONNX IR to Compute IR
  virtual void ISelLowering(const ::onnx::Graph *onnxGraph);

  virtual void
  LowerOperation(const ::onnx::Node &node,
                 std::vector<std::unique_ptr<ComputeOperand2> > &instList) = 0;

protected:
  TGBackend *m_pBackend;
};

} // namespace onnc
