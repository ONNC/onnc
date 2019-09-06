//===- LiveIntervalsData.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <cassert>

#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Support/Memory.h>

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
  assert(hasInterval(pV) && "Must have matched interval before get.");

  return m_ValIntrvls.find(const_cast<Value*>(pV))->second.get();
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

  for (auto& entry : m_ValIntrvls)
    liveIntrvls.push_back(entry.second.get());

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
  m_ValIntrvls.clear();
}

LiveInterval* LiveIntervalsData::createEmptyLiveInterval(Value* pV)
{
  assert(!hasInterval(pV) && "Live interval has been created.");
  auto result = m_ValIntrvls.emplace(pV, std::make_unique<LiveInterval>(pV));
  const auto& entry = *result.first;
  return entry.second.get();
}

//===----------------------------------------------------------------------===//
// LiveIntervalsData Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(LiveIntervalsData, "LiveIntervalsData")
}

LiveIntervalsData* onnc::CreateLiveIntervalsDataPass()
{
  return new LiveIntervalsData();
}
