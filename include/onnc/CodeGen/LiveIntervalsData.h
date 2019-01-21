//===- LiveIntervalsData.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LIVE_INTERVALS_DATA_H
#define ONNC_CODEGEN_LIVE_INTERVALS_DATA_H
#include <memory>

#include <onnc/CodeGen/LiveInterval.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {

class BuildSlotIndexes;

/** \class LiveIntervalsData
 *  \brief A data pass containing live intervals of all onnc::Value in an
 *         onnc::Module. Provide utilities for live interval queries.
 */
class LiveIntervalsData : public CustomPass<LiveIntervalsData>
{
public:
  typedef std::unordered_map<Value*, std::unique_ptr<LiveInterval>> ValueIntervalMap;

  typedef std::vector<LiveInterval*> LIs;

public:
  LiveIntervalsData()
    : m_SlotIndexes(nullptr) {
  }

  virtual ~LiveIntervalsData() = default;

  //===--------------------------------------------------------------------===//
  // Pass methods.
  //===--------------------------------------------------------------------===//
  ReturnType runOnModule(Module& pModule) override { return kModuleNoChanged; }

  StringRef getPassName() const override { return "LiveIntervalsData"; }

  void print(OStream& pOS, const Module* pModule) const override;

  //===--------------------------------------------------------------------===//
  // Query interface.
  //===--------------------------------------------------------------------===//
  bool hasInterval(const Value* pV) const;

  const LiveInterval* getInterval(const Value* pV) const;

  /// Get all live intervals and sort them by start slot index in increasing
  /// order.
  const LIs getSortedIntervals() const;

  unsigned getNumSlots() const { return m_SlotIndexes->getNumSlots(); }

  SlotIndex getSlotIndex(const ComputeOperator* pOp) const
  {
    return m_SlotIndexes->getSlotIndex(pOp);
  }

  //===--------------------------------------------------------------------===//
  // Add/Remove live interval entry interface.
  //===--------------------------------------------------------------------===//
  LiveInterval* createEmptyLiveInterval(Value* pV);

  /// Remove a live interval pV from liveness table.
  void removeLiveInterval(const Value* pV);

  /// Get internal data structure.
  ValueIntervalMap& getAllIntervals() { return m_ValIntrvls; }

  void setSlotIndexes(BuildSlotIndexes* pSlotIndex)
  {
    m_SlotIndexes = pSlotIndex;
  }

  /// Delete LiveInterval object.
  void clear() override;

private:
  ValueIntervalMap m_ValIntrvls;
  BuildSlotIndexes* m_SlotIndexes;
};

LiveIntervalsData* CreateLiveIntervalsDataPass();

} // namespace of onnc

#endif
