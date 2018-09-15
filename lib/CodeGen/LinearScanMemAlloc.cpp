//===- LinearScanMemAlloc.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetMemInfo.h>
#include <iomanip>

using namespace onnc;

/// Check if two segment A overlaps segment B.
///   segment A = [pStartA, pStartA + pSizeA)
///   segment B = [pStartB, pStartB + pSizeB)
static bool HasConflict(uint64_t pStartA, uint64_t pSizeA,
                        uint64_t pStartB, uint64_t pSizeB)
{
  uint64_t endA = pStartA + pSizeA,
           endB = pStartB + pSizeB;

  return !(endA <= pStartB || endB <= pStartA);
}

static uint64_t GetAlignedAddr(uint64_t pAddr, uint64_t pAlignment)
{
  assert(pAlignment > 0 && ((pAlignment & (pAlignment - 1)) == 0) &&
         "Alignment should be powerof 2.");
  pAddr += pAlignment - 1;
  pAddr &= ~(pAlignment - 1);
  return pAddr;
}

//===----------------------------------------------------------------------===//
// LinearScanMemAlloc
//===----------------------------------------------------------------------===//
LinearScanMemAlloc::LinearScanMemAlloc(TargetBackend* pTarget)
  : ModulePass(ID), m_ValToAllocEntry(),
    m_LIPass(nullptr), m_LiveMatPass(nullptr) {
  m_TMI = pTarget->getMemInfo();
}

Pass::ReturnType LinearScanMemAlloc::runOnModule(Module& pModule)
{
  m_LIPass = getAnalysis<LiveIntervals>();
  m_LiveMatPass = getAnalysis<LiveValueMatrix>();

  AllocEntries curAllocs;

  // Allocate memory for each value (live interval).
  for (const LiveInterval* LI: m_LIPass->getSortedIntervals()) {
    LiveIntervals::LIs overlappedLIs =
      m_LiveMatPass->getInterferingLiveIntervals(LI);

    AllocEntries allocRegions = getSortedAllocatedRegions(overlappedLIs);

    Value* v = const_cast<Value*>(LI->getValue());

    // FIXME: Do we have safer casting? We should check before casting.
    MemSize m = m_TMI->getTensorMemorySize(*(Tensor*)v);

    m_ValToAllocEntry[v] =
      getAnEmptyRegion(allocRegions, m.size, m.alignment);
  }
  return Pass::kModuleNoChanged;
}

void LinearScanMemAlloc::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(LiveIntervals::ID);
  pUsage.addRequiredID(LiveValueMatrix::ID);
}

LinearScanMemAlloc::AllocEntry
LinearScanMemAlloc::getAlloc(const Value* pVal) const
{
  auto it = m_ValToAllocEntry.find(const_cast<Value*>(pVal));
  assert(it != m_ValToAllocEntry.end() &&
         "Value has no memory allocation.");
  return it->second;
}

void LinearScanMemAlloc::print(std::ostream& pOS) const
{
  pOS << "=== LinearScanMemAlloc ===\n";

  if (m_ValToAllocEntry.empty()) {
    pOS << "Empty.\n";
    return;
  }

  std::stringstream dbgstr;
  dbgstr << std::hex << std::left
         << std::setw(20) << "value:" << std::setw(12) << "start"
         << std::setw(12) << "end"
         << "\n";

  uint64_t maxEnd = 0;

  for (const LiveInterval* li : m_LIPass->getSortedIntervals()) {
    Value* v = const_cast<Value*>(li->getValue());
    AllocEntry alloc = m_ValToAllocEntry.find(v)->second;
    uint64_t endAddr = alloc.startAddr + alloc.size;
    maxEnd = std::max(maxEnd, endAddr);

    dbgstr << std::left << std::setw(20) << std::setfill(' ') << v->getName()
           << "0x"
           << std::right << std::setw(8) << std::setfill('0') << alloc.startAddr
           << "  0x" << std::setw(8) << endAddr
           << "\n";
  }

  dbgstr << std::dec << "\nTotal memory usages = "
         << (double)maxEnd / (1024.0 * 1024.0) << " mb\n";

  pOS << dbgstr.str();
}

void LinearScanMemAlloc::dump() const { print(errs()); }

LinearScanMemAlloc::AllocEntries
LinearScanMemAlloc::getSortedAllocatedRegions(const LIs& pLIs) const
{
  AllocEntries allocs;
  for (const LiveInterval* li : pLIs) {
    Value* v = const_cast<Value*>(li->getValue());
    auto entIt = m_ValToAllocEntry.find(v);
    if (entIt != m_ValToAllocEntry.end())
      allocs.push_back(entIt->second);
  }

  // sort by starting address
  std::sort(allocs.begin(), allocs.end(),
            [] (const AllocEntry& pA, const AllocEntry& pB) {
              return pA.startAddr < pB.startAddr;
            });

  return allocs;
}

LinearScanMemAlloc::AllocEntry
LinearScanMemAlloc::getAnEmptyRegion(
  const LinearScanMemAlloc::AllocEntries& pAllocs,
  uint64_t pRequiredSize, uint64_t pAlignment) const
{
  uint64_t startAddr = 0;
  for (const AllocEntry& alloc : pAllocs) {
    if (HasConflict(alloc.startAddr, alloc.size, startAddr, pRequiredSize)) {
      startAddr = GetAlignedAddr(alloc.startAddr + alloc.size, pAlignment);
      continue;
    }

    // Ok we find an empty region.
    //
    // Note! pAllocs is sorted by startAddr, so if end address of new allocation
    // is <= current allocated start address, that means we won't have conflict
    // with other unchecked regions since these regions must have start address
    // > new allocation's end address.
    if ((startAddr + pRequiredSize) <= alloc.startAddr)
      break;
  }
  return AllocEntry(startAddr, pRequiredSize);
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char LinearScanMemAlloc::ID = 0;

namespace onnc
{
  INITIALIZE_TB_PASS(LinearScanMemAlloc, "LinearScanMemAlloc")
}

ModulePass* onnc::CreateLinearScanMemAllocPass(TargetBackend* pTB)
{
  return new LinearScanMemAlloc(pTB);
}