//===- LiveInterval.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LiveInterval.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveRange
//===----------------------------------------------------------------------===//
LiveRange::LiveRange()
  : m_Segments() {
}

void LiveRange::addSegment(const Segment &pNewSeg)
{
  for (auto& seg : m_Segments)
    assert(seg.overlap(pNewSeg) && "Can't add overlapped segment.");

  m_Segments.push_back(pNewSeg);
}

SlotIndex LiveRange::beginIndex() const
{
  return m_Segments.front().m_Start;
}

SlotIndex LiveRange::endIndex() const
{
  return m_Segments.back().m_End;
}

bool LiveRange::overlap(const LiveRange& pOther) const
{
  for (auto& m : m_Segments)
    for (auto& n : pOther.m_Segments)
      if (m.overlap(n))
        return true;
  return false;
}

//===----------------------------------------------------------------------===//
// Segment
//===----------------------------------------------------------------------===//
bool LiveRange::Segment::overlap(const Segment& pOther) const
{
  if (m_Start > pOther.m_End || m_End < pOther.m_Start)
    return false;
  return true;
}

//===----------------------------------------------------------------------===//
// LiveInterval
//===----------------------------------------------------------------------===//
void LiveInterval::print(std::ostream& pOS) const
{
  pOS << m_pValue->getName() << ":";
  for (auto& seg : m_Segments)
    pOS << " [" << seg.m_Start.getIndex()
        << ", " << seg.m_End.getIndex() << "]";
  pOS << "\n";
}

void LiveInterval::dump() const { print(errs()); }