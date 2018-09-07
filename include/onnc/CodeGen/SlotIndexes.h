//===- SlotIndexes.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SLOT_INDEXES_H
#define ONNC_SLOT_INDEXES_H
#include <onnc/ADT/IList.h>
#include <onnc/ADT/IListNode.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

/** \class TimeSlot
 *  \brief TimeSlot presents the index of a time slot in the liveness table
 *         The class should not be used directly.
 */
class TimeSlot : public IListNode<TimeSlot>
{
public:
  TimeSlot(ComputeOperator& pOp, unsigned int pIdx)
    : m_Idx(pIdx), m_pOperator(&pOp) {
  }

  unsigned int getIndex() const { return m_Idx; }

  ComputeOperator* getOperator() { return m_pOperator; }

  const ComputeOperator* getOperator() const { return m_pOperator; }

private:
  unsigned int m_Idx;
  ComputeOperator* m_pOperator;
};

/** \class SlotIndex
 *  \brief SlotIndex represents a pointer to a time slot.
 */
class SlotIndex
{
public:
  SlotIndex(TimeSlot* pSlot)
    : m_pTimeSlot(pSlot) {
  }

  bool operator>(const SlotIndex& pOther) const
  {
    return m_pTimeSlot->getIndex() > pOther.m_pTimeSlot->getIndex();
  }

  bool operator<(const SlotIndex& pOther) const
  {
    return m_pTimeSlot->getIndex() < pOther.m_pTimeSlot->getIndex();
  }

  unsigned getIndex() const { return m_pTimeSlot->getIndex(); }

private:
  TimeSlot* m_pTimeSlot;
};

/** \class BuildSlotIndexes
 *  \brief Assign slot index to each ComputeOperator, slot index is used by
 *         liveness related passes.
 */
class BuildSlotIndexes : public ModulePass
{
public:
  typedef IList<TimeSlot> TimeSlotList;
  typedef std::unordered_map<ComputeOperator*, SlotIndex> COpToSlotIdxMap;

  static char ID;

  static constexpr unsigned OperatorDist = 1;

public:
  BuildSlotIndexes()
    : ModulePass(ID), m_TSList(), m_StartIdx(0) {
  }

  virtual ~BuildSlotIndexes()
  {
    clear();
  }

  ReturnType runOnModule(Module& pModule) override;

  void runOnComputeGraph(ComputeGraph& pCG);

  bool hasSlotIndex(const ComputeOperator* pOp) const;

  SlotIndex getSlotIndex(const ComputeOperator* pOp) const;

  size_t getNumSlots() const { return m_COpToSlotIdx.size(); }

  StringRef getPassName() const override { return "BuildSlotIndexes"; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

private:
  TimeSlot* createTimeSlot(ComputeOperator& pOp, unsigned pIdx);

  /// delete TimeSlots of m_TSList, clear containers, and reset start index.
  void clear();

private:
  TimeSlotList m_TSList;
  unsigned m_StartIdx;
  COpToSlotIdxMap m_COpToSlotIdx;
};

BuildSlotIndexes* CreateBuildSlotIndexesPass();

} // namespace of onnc

#endif