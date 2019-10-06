//===- LiveValueMatrix.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <iomanip>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LiveValueMatrix
//===----------------------------------------------------------------------===//
const LiveValueMatrix::LIs&
LiveValueMatrix::getInterferingLiveIntervals(const Value* pV) const
{
  return getInterferingLiveIntervals(m_LIDataPass->getInterval(pV));
}

const LiveValueMatrix::LIs&
LiveValueMatrix::getInterferingLiveIntervals(const LiveInterval* pLI) const
{
  LiveInterval* li = const_cast<LiveInterval*>(pLI);
  assert(m_OverlapLIs.find(li) != m_OverlapLIs.end() &&
         "Unknow LiveInterval.");
  return m_OverlapLIs.find(li)->second;
}

Pass::ReturnType LiveValueMatrix::runOnModule(Module& pModule)
{
  clear();

  m_LIDataPass = getAnalysis<LiveIntervalsData>();
  buildStartWithEndWith();

  // Have to construct overlap set in compute operator order
  // (slotIndex in increasing order)
  const unsigned numSlots = m_LIDataPass->getNumSlots();
  for (unsigned i = 0; i < numSlots; ++i)
    calculateOverlapSet(i);

  return Pass::kModuleNoChanged;
}

void LiveValueMatrix::calculateOverlapSet(unsigned pSlotIdx)
{
  for (LiveSegment& curLS : m_StartWith[pSlotIdx]) {
    // For each live segment 'curLS' which starts at ComputeOperator i:
    // Because curLS has overlap with current LiveIntervalSet, so update
    // m_OverlapLIs for curLS and current LiveIntervalSet.
    for (auto& liSetIter : m_CurLISet) {
      m_OverlapLIs[curLS.m_LI].push_back(liSetIter.first);
      m_OverlapLIs[liSetIter.first].push_back(curLS.m_LI);
    }

    // Add user count for LiveInterval which owns this LiveSegment.
    ++m_CurLISet[curLS.m_LI];
  }

  for (LiveSegment& curLS : m_EndWith[pSlotIdx]) {
    assert(m_CurLISet[curLS.m_LI] > 0 && "Invalid LiveSegment count.");
    --m_CurLISet[curLS.m_LI];
    // Erase LiveInterval from current LiveIntervalSet because it has no users.
    if (m_CurLISet[curLS.m_LI] == 0)
      m_CurLISet.erase(curLS.m_LI);
  }
}

void LiveValueMatrix::buildStartWithEndWith()
{
  const unsigned numSlots = m_LIDataPass->getNumSlots();
  m_StartWith.resize(numSlots);
  m_EndWith.resize(numSlots);

  // m_StartWith (m_EndWith) is accessed by ComputeOperator's SlotIndex, if
  // OperatorDist is not 1, we need an index function that transform SlotIndex
  // to array index, something like llvm::IndexedMap.
  static_assert(BuildSlotIndexes::OperatorDist == 1,
                "Assume OperatorDist is 1.");

  for (auto& liIter : m_LIDataPass->getAllIntervals()) {
    auto& li = liIter.second;
    // Initialize overlap set through operator[]
    m_OverlapLIs.emplace(li.get(), std::vector<onnc::LiveInterval*>());
    for (const LiveInterval::Segment& s : li->getSegments()) {
      m_StartWith[s.m_Start.getIndex()].emplace_back(li.get(), s);
      m_EndWith[s.m_End.getIndex()].emplace_back(li.get(), s);
    }
  }
}

void LiveValueMatrix::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequired<LiveIntervalsData>();
}

void LiveValueMatrix::print(OStream& pOS, const Module* pModule) const
{
  pOS << "=== Live Matrix (Interference) ===\n";
  if (m_OverlapLIs.empty()) {
    pOS << "Empty.\n";
    return;
  }

  std::stringstream dbgstr;
  for (const LiveInterval* li : m_LIDataPass->getSortedIntervals()) {
    dbgstr << std::left << std::setw(20) << li->getValue()->getName() << ":";
    for (const LiveInterval* overlapLI : getInterferingLiveIntervals(li))
      dbgstr << " " << overlapLI->getValue()->getName();
    dbgstr << "\n";
  }
  pOS << dbgstr.str();
}

void LiveValueMatrix::clear()
{
  m_OverlapLIs.clear();
  m_StartWith.clear();
  m_EndWith.clear();
  m_CurLiveSegSet.clear();
  m_CurLISet.clear();
  m_LIDataPass = nullptr;
}

//===----------------------------------------------------------------------===//
// BuildSlotIndexes Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(LiveValueMatrix, "LiveValueMatrix")
}

LiveValueMatrix* onnc::CreateLiveValueMatrixPass()
{
  return new LiveValueMatrix();
}
