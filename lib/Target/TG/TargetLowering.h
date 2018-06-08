#pragma once

#include "ComputeOperator.h"
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
  using ComputeGraph = std::vector<std::unique_ptr<ComputeOperator2> >;

public:
  TargetLowering(TGBackend *pBackend) : m_pBackend(pBackend) {}

  virtual ~TargetLowering() = default;

  // do something before ISelLowering
  virtual void PrepareISelLowering(Module &pModule) { return; };

  // Lowering ONNX IR to Compute IR
  virtual void ISelLowering(const ::onnx::Graph *onnxGraph);

  virtual ComputeOperator2 *LowerOperation(const ::onnx::Node &node,
                                          ComputeGraph &graph) = 0;

protected:
  TGBackend *m_pBackend;
};

} // namespace onnc
