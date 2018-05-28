//===- NodeIRScheduler.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements IR scheduler, especially focus on reorder load/store to
// appropriate position for max performance.
//
//===----------------------------------------------------------------------===//
#ifndef NODE_IR_SCHEDULER_H
#define NODE_IR_SCHEDULER_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnx/common/ir.h>

namespace onnc {

class NodeIRScheduler : public ModulePass
{
public:
  static char ID;

  virtual ~NodeIRScheduler();

public:
  NodeIRScheduler(DLATargetBackend* pDLATB = nullptr);

  ReturnType runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  void print(std::ostream& pOS) const;

private:
  DLATargetBackend* m_DLATB;
};

NodeIRScheduler *CreateNodeIRSchedulerPass(DLATargetBackend *pDLATB);

}  // namespace of onnc

#endif  // NODE_IR_SCHEDULER_H
