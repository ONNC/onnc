//===- LiveIntervals.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/PassAnalysisSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveIntervals
//===----------------------------------------------------------------------===//
Pass::ReturnType LiveIntervals::runOnModule(Module& pModule)
{
  clear();

  m_SlotIndexes = getAnalysis<BuildSlotIndexes>();

  for (auto& valIt : pModule.getValueList()) {
    Value* v = valIt.value();
    LiveInterval* liveintrvl = createEmptyLiveInterval(v);
    computeValueInterval(*liveintrvl);
  }

  return Pass::kModuleNoChanged;
}

void LiveIntervals::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(BuildSlotIndexes::ID);
}

bool LiveIntervals::hasInterval(const Value* pV) const
{
  return m_ValIntrvls.find(const_cast<Value*>(pV)) != m_ValIntrvls.end();
}

const LiveInterval* LiveIntervals::getInterval(const Value* pV) const
{
  return m_ValIntrvls.find(const_cast<Value*>(pV))->second;
}

void LiveIntervals::removeLiveInterval(const Value* pV)
{
  assert(hasInterval(pV) && "The value has no interval.");
  m_ValIntrvls.erase(const_cast<Value*>(pV));
}

const LiveIntervals::LIs LiveIntervals::getSortedIntervals() const
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

void LiveIntervals::print(OStream& pOS, const Module* pModule) const
{
  pOS << "=== Live Intervals ===\n";
  if (m_ValIntrvls.empty()) {
    pOS << "Empty.\n";
    return;
  }

  std::stringstream dbgstr;
  for (const LiveInterval* li : getSortedIntervals())
    li->print(dbgstr);

  pOS << dbgstr.str();
}

void LiveIntervals::clear()
{
  for (auto liIter : m_ValIntrvls) {
    LiveInterval* li = liIter.second;
    delete li;
  }
  m_ValIntrvls.clear();
}

LiveInterval* LiveIntervals::createEmptyLiveInterval(Value* pV)
{
  assert(!hasInterval(pV) && "Live interval has been created.");
  LiveInterval* li = new LiveInterval(pV);
  m_ValIntrvls[pV] = li;
  return li;
}

void LiveIntervals::computeValueInterval(LiveInterval& pLI)
{
  Value* v = pLI.getValue();
  // FIXME: Can I assume (ComputeOperator*)v->getDefine() is true?
  SlotIndex start = m_SlotIndexes->getSlotIndex(
    (ComputeOperator*)v->getDefine());
  SlotIndex end = start;

  // find the last user
  for (const Use& u : v->getUses())
    end = std::max(end, m_SlotIndexes->getSlotIndex(u.getUser()));

  pLI.addSegment(LiveRange::Segment(start, end));
}

//===----------------------------------------------------------------------===//
// LiveIntervals Factory method
//===----------------------------------------------------------------------===//
char LiveIntervals::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(LiveIntervals, "LiveIntervals")
}

LiveIntervals* onnc::CreateLiveIntervalsPass()
{
  return new LiveIntervals();
}