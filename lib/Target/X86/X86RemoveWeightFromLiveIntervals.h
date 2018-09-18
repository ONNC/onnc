//===- X86RemoveWeightFromLiveIntervals.h ---------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_REMOVE_WEIGHT_FROM_LIVE_INTERVALS_H
#define TARGET_X86_X86_REMOVE_WEIGHT_FROM_LIVE_INTERVALS_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

/** \class X86RemoveWeightFromLiveIntervals
 *  \brief X86 interpreter use mmap to access weights data, so memory allocation
 *         pass doesn't have to allocate memory for weights. This pass remove
 *         weights from live interval.
 */
class X86RemoveWeightFromLiveIntervals : public ModulePass
{
public:
  static char ID;

public:
  X86RemoveWeightFromLiveIntervals()
    : ModulePass(ID) {
  }

  ReturnType runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;
};

X86RemoveWeightFromLiveIntervals* CreateX86RemoveWeightFromLiveIntervalsPass();

} // namespace of onnc

#endif
