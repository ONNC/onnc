//===- LiveInterval.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LIVE_INTERVAL_H
#define ONNC_CODEGEN_LIVE_INTERVAL_H
#include <onnc/ADT/IListNode.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/IR/Compute/Value.h>

namespace onnc {

/** \class LiveRange
 *  \brief LiveRange represents the liveness of a value.
 */
class LiveRange
{
public:
  /** \class Segment
   *  This represents a simple continuous liveness interval for a value.
   *  The start point and end point are both inclusive. These intervals
   *  are rendered as [start,end].
   */
  struct Segment
  {
    SlotIndex m_Start;  // Start point of the interval (inclusive)
    SlotIndex m_End;    // End point of the interval (inclusive)

    Segment(SlotIndex pStart, SlotIndex pEnd)
      : m_Start(pStart), m_End(pEnd) {
      assert(m_Start < m_End && "Invalid liverange segment.");
    }

    /// Return true if the two segments has overlap.
    bool overlap(const Segment& pOther) const;
  };

  typedef std::vector<Segment> Segments;

public:
  LiveRange();

  virtual ~LiveRange() {}

  /// Add a new segment, which does not overlap with existing segments.
  void addSegment(const Segment &pNewSeg);

  /// Return the lowest slot index.
  SlotIndex beginIndex() const;

  /// Return the highest slot index.
  SlotIndex endIndex() const;

protected:
  Segments m_Segments;
};

/** \class LiveInterval
 *  \brief LiveInterval represents the liveness of a value.
 */
class LiveInterval : public LiveRange
{
public:
  LiveInterval(Value* pV)
    : m_pValue(pV) {
  }

  Value* getValue() { return m_pValue; }

  void print(std::ostream& pOS) const;

  void dump() const;

private:
  Value* m_pValue;
};

} // namespace of onnc

#endif
