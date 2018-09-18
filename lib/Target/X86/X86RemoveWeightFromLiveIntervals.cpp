//===- X86RemoveWeightFromLiveIntervals.cpp -------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86RemoveWeightFromLiveIntervals.h"
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/IR/Compute/Initializer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86RemoveWeightFromLiveIntervals
//===----------------------------------------------------------------------===//
Pass::ReturnType X86RemoveWeightFromLiveIntervals::runOnModule(Module& pModule)
{
  LiveIntervals* liveIntrvlPass = getAnalysis<LiveIntervals>();

  for (auto& valIt : pModule.getValueList()) {
    Value* v = valIt.value();
    // TODO: check if Define is ComputeOperator before casting.
    ComputeOperator* op = static_cast<ComputeOperator*>(v->getDefine());
    // If the value is weight, remove it from liveness table.
    if (isa<Initializer>(op))
      liveIntrvlPass->removeLiveInterval(v);
  }

  return Pass::kModuleNoChanged;
}

void X86RemoveWeightFromLiveIntervals::getAnalysisUsage(
  AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(LiveIntervals::ID);
}

//===----------------------------------------------------------------------===//
// X86RemoveWeightFromLiveIntervals Factory method
//===----------------------------------------------------------------------===//
char X86RemoveWeightFromLiveIntervals::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(X86RemoveWeightFromLiveIntervals,
                  "X86RemoveWeightFromLiveIntervals")
}

X86RemoveWeightFromLiveIntervals*
onnc::CreateX86RemoveWeightFromLiveIntervalsPass()
{
  return new X86RemoveWeightFromLiveIntervals();
}
