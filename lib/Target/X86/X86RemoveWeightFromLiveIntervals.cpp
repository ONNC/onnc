//===- X86RemoveWeightFromLiveIntervals.cpp -------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86RemoveWeightFromLiveIntervals.h"
#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86RemoveWeightFromLiveIntervals
//===----------------------------------------------------------------------===//
Pass::ReturnType X86RemoveWeightFromLiveIntervals::runOnModule(Module& pModule)
{
  LiveIntervalsData* liveIntrvlPass = getAnalysis<LiveIntervalsData>();

  for (auto& valIt : pModule.getValueList()) {
    Value* v = valIt.value();
    // TODO: check if Define is ComputeOperator before casting.
    ComputeOperator* op = static_cast<ComputeOperator*>(v->getDefine());
    // If the value is weight, remove it from liveness table.
    if (isa<Initializer>(op) || isa<InputOperator>(op))
      liveIntrvlPass->removeLiveInterval(v);
  }

  return Pass::kModuleNoChanged;
}

void X86RemoveWeightFromLiveIntervals::getAnalysisUsage(
  AnalysisUsage& pUsage) const
{
  pUsage.addRequired<LiveIntervalsData>();
}

namespace onnc
{
  INITIALIZE_PASS(X86RemoveWeightFromLiveIntervals,
                  "X86RemoveWeightFromLiveIntervals")
}
