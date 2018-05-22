#pragma once

#include "Operator.h"
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

  // do something before CodeGenAndEmitInst
  virtual void PrepareCodeGenAndEmitInst(Module &pModule) { return; };

  virtual void CodeGenAndEmitInst(const ::onnx::Graph *onnxGraph);

  virtual void
  LowerOperation(const ::onnx::Node &node,
                 std::vector<std::unique_ptr<Operator> > &instList) = 0;

protected:
  TGBackend *m_pBackend;
};

} // namespace onnc
