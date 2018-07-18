//===- LiveInterval.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LIVE_INTERVAL_H
#define ONNC_CODEGEN_LIVE_INTERVAL_H
#include <onnc/IR/Compute/Value.h>
#include <onnc/ADT/IListNode.h>

namespace onnc {

/** \class ValueIndex
 *  \brief ValueIndex represents the value number in the liveness table.
 */
class ValueIndex : public IListNode<ValueIndex>
{
public:
  ValueIndex(onnc::Value& pValue);

  onnc::Value* value() { return m_pValue; }

  const onnc::Value* value() const { return m_pValue; }

  bool isMe(const onnc::Value& pV) const { return (&pV == m_pValue); }

private:
  onnc::Value* m_pValue;
};

/** \class TimeSlot
 *  \brief TimeSlot preresents the index of a time slot in the liveness table
 */
class TimeSlot : public IListNode<TimeSlot>
{
public:
  TimeSlot(ComputeOperator& pOp, unsigned int pIdx);

  unsigned int getIndex() const { return pIdx; }

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
  SlotIndex();

  SlotIndex(TimeSlot* pSlot);

private:
  TimeSlot* m_pTimeSlot;
};

/** \class LiveRange
 *  \brief LiveRange represents the liveness of a value.
 *
 *  This represents a simple continuous liveness interval for a value.
 *  The start point is inclusive, the end point exclusive. These intervals
 *  are rendered as [start,end).
 */
class LiveRange
{
public:
  LiveRange();

private:
  SlotIndex m_Start;  // Start point of the interval (inclusive)
  SlotIndex m_End;    // End point of the interval (exclusive)
  ValueIndex* m_ValueIndex; // identifier for the value contained in this
};

} // namespace of onnc

#endif
