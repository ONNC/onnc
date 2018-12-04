//===- LiveIntervals.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LIVE_INTERVALS_H
#define ONNC_CODEGEN_LIVE_INTERVALS_H
#include <onnc/CodeGen/LiveInterval.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {

/** \class LiveIntervals
 *  \brief Build live intervals for all onnc::Value of onnc::Module.
 *         Save the building result in LiveIntervalsData.
 */
class LiveIntervals : public ModulePass
{
public:
  static char ID;

public:
  LiveIntervals() : ModulePass(LiveIntervals::ID) {}

  ReturnType runOnModule(Module& pModule) override;

  void runOnComputeGraph(Module& pModule, ComputeGraph& pCG,
                         BuildSlotIndexes& pBuildSlotIndexes);

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  StringRef getPassName() const override { return "LiveIntervals"; }

  void print(OStream& pOS, const Module* pModule) const override;

private:
  void computeValueInterval(LiveInterval& pLI,
                            const BuildSlotIndexes& pSlotIndexes);
};

LiveIntervals* CreateLiveIntervalsPass();

} // namespace of onnc

#endif