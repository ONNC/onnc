//===- LiveIntervals.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/Core/PassAnalysisSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveIntervals
//===----------------------------------------------------------------------===//
Pass::ReturnType LiveIntervals::runOnModule(Module& pModule)
{
  BuildSlotIndexes& slotIndexes = *getAnalysis<BuildSlotIndexes>();
  LiveIntervalsData& liData = *getAnalysis<LiveIntervalsData>();
  liData.setSlotIndexes(&slotIndexes);

  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg) {
    ComputeGraph::iterator nodeIt, nEnd = cg->value()->end();
    for (nodeIt = cg->value()->begin(); nodeIt != nEnd; ++nodeIt) {
      for (int i = 0; i < nodeIt->getNumOfOutputs(); ++i) {
        Value * const value = nodeIt->getOutput(i);
        assert(value != nullptr);

        if (liData.hasInterval(value)) {
          continue;
        }

        LiveInterval * const interval = liData.createEmptyLiveInterval(value);
        computeValueInterval(*interval, slotIndexes);
      }
    }
  }

  return Pass::kModuleNoChanged;
}

void LiveIntervals::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequired<BuildSlotIndexes>();
  pUsage.addRequired<LiveIntervalsData>();
}

void LiveIntervals::print(OStream& pOS, const Module* pModule) const
{
  LiveIntervalsData& liData = *getAnalysis<LiveIntervalsData>();
  liData.print(pOS, pModule);
}

void LiveIntervals::computeValueInterval(LiveInterval& pLI,
                                         const BuildSlotIndexes& pSlotIndexes)
{
  Value* v = pLI.getValue();
  // FIXME: Can I assume (ComputeOperator*)v->getDefine() is true?
  ComputeOperator* op = static_cast<ComputeOperator*>(v->getDefine());
  SlotIndex start = pSlotIndexes.getSlotIndex(op);
  SlotIndex end = start;

  // find the last user
  for (const Use& u : v->getUses())
    end = std::max(end, pSlotIndexes.getSlotIndex(u.getUser()));

  pLI.addSegment(LiveRange::Segment(start, end));
}

//===----------------------------------------------------------------------===//
// LiveIntervals Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(LiveIntervals, "LiveIntervals")
}

LiveIntervals* onnc::CreateLiveIntervalsPass()
{
  return new LiveIntervals();
}
