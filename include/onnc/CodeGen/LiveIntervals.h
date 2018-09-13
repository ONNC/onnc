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

class BuildSlotIndexes;

/** \class LiveIntervals
 *  \brief Build live intervals for all onnc::Value of onnc::Module.
 *         Provide utilities for live interval queries.
 */
class LiveIntervals : public ModulePass
{
public:
  static char ID;

  typedef std::unordered_map<Value*, LiveInterval*> ValueIntervalMap;

  typedef std::vector<LiveInterval*> LIs;

public:
  LiveIntervals()
    : ModulePass(LiveIntervals::ID), m_ValIntrvls(), m_SlotIndexes(nullptr) {
  }

  virtual ~LiveIntervals() { clear(); }

  ReturnType runOnModule(Module& pModule) override;

  void runOnComputeGraph(Module& pModule, ComputeGraph& pCG,
                         BuildSlotIndexes& pBuildSlotIndexes);

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  StringRef getPassName() const override { return "LiveIntervals"; }

  bool hasInterval(const Value* pV) const;

  const LiveInterval* getInterval(const Value* pV) const;

  /// Get internal data structure.
  ValueIntervalMap& getAllIntervals() { return m_ValIntrvls; }

  /// Get all live intervals and sort them by start slot index in increasing
  /// order.
  const LIs getSortedIntervals() const;

  unsigned getNumSlots() const { return m_SlotIndexes->getNumSlots(); }

  SlotIndex getSlotIndex(const ComputeOperator* pOp) const
  {
    return m_SlotIndexes->getSlotIndex(pOp);
  }

  void print(std::ostream& pOS) const;

  void dump() const;

private:
  /// Delete LiveInterval object.
  void clear();

  LiveInterval* createEmptyLiveInterval(Value* pV);

  void computeValueInterval(LiveInterval& pLI);

private:
  ValueIntervalMap m_ValIntrvls;
  BuildSlotIndexes* m_SlotIndexes;
};

LiveIntervals* CreateLiveIntervalsPass();

} // namespace of onnc

#endif