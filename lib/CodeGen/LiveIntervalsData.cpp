//===- LiveIntervalsData.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/PassAnalysisSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveIntervalsData
//===----------------------------------------------------------------------===//
bool LiveIntervalsData::hasInterval(const Value* pV) const
{
  return m_ValIntrvls.find(const_cast<Value*>(pV)) != m_ValIntrvls.end();
}

const LiveInterval* LiveIntervalsData::getInterval(const Value* pV) const
{
  return m_ValIntrvls.find(const_cast<Value*>(pV))->second;
}

void LiveIntervalsData::removeLiveInterval(const Value* pV)
{
  assert(hasInterval(pV) && "The value has no interval.");
  m_ValIntrvls.erase(const_cast<Value*>(pV));
}

const LiveIntervalsData::LIs LiveIntervalsData::getSortedIntervals() const
{
  LIs liveIntrvls;
  liveIntrvls.reserve(m_ValIntrvls.size());

  for (auto liIter : m_ValIntrvls)
    liveIntrvls.push_back(liIter.second);

  // sort the index in increasing order.
  std::sort(liveIntrvls.begin(), liveIntrvls.end(),
            [] (const LiveInterval* a, const LiveInterval* b) {
              return a->beginIndex() < b->beginIndex();
            });
  return liveIntrvls;
}

void LiveIntervalsData::print(OStream& pOS, const Module* pModule) const
{
  pOS << "=== Live Intervals Data ===\n";
  if (m_ValIntrvls.empty()) {
    pOS << "Empty.\n";
    return;
  }

  std::stringstream dbgstr;
  for (const LiveInterval* li : getSortedIntervals())
    li->print(dbgstr);

  pOS << dbgstr.str();
}

void LiveIntervalsData::clear()
{
  for (auto liIter : m_ValIntrvls) {
    LiveInterval* li = liIter.second;
    delete li;
  }
  m_ValIntrvls.clear();
}

LiveInterval* LiveIntervalsData::createEmptyLiveInterval(Value* pV)
{
  assert(!hasInterval(pV) && "Live interval has been created.");
  LiveInterval* li = new LiveInterval(pV);
  m_ValIntrvls[pV] = li;
  return li;
}

//===----------------------------------------------------------------------===//
// LiveIntervalsData Factory method
//===----------------------------------------------------------------------===//
char LiveIntervalsData::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(LiveIntervalsData, "LiveIntervalsData")
}

LiveIntervalsData* onnc::CreateLiveIntervalsDataPass()
{
  return new LiveIntervalsData();
}
