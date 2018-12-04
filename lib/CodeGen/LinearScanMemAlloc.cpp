//===- LinearScanMemAlloc.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervalsData.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetMemInfo.h>

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
  : ModulePass(ID), m_MemAllocData(nullptr),
    m_LIDataPass(nullptr), m_LiveMatPass(nullptr) {
  m_TMI = pTarget->getMemInfo();
}

Pass::ReturnType LinearScanMemAlloc::runOnModule(Module& pModule)
{
  m_LIDataPass = getAnalysis<LiveIntervalsData>();
  m_LiveMatPass = getAnalysis<LiveValueMatrix>();
  m_MemAllocData = getAnalysis<MemAllocData>();

  // Allocate memory for each value (live interval).
  for (const LiveInterval* LI: m_LIDataPass->getSortedIntervals()) {
    LiveIntervalsData::LIs overlappedLIs =
      m_LiveMatPass->getInterferingLiveIntervals(LI);

    AllocEntries allocRegions = getSortedAllocatedRegions(overlappedLIs);

    Value* v = const_cast<Value*>(LI->getValue());

    // FIXME: Do we have safer casting? We should check before casting.
    MemSize m = m_TMI->getTensorMemorySize(*(Tensor*)v);

    m_MemAllocData->addAlloc(v, getAnEmptyRegion(allocRegions,
                                                 m.size, m.alignment));
  }
  return Pass::kModuleNoChanged;
}

void LinearScanMemAlloc::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(LiveIntervalsData::ID);
  pUsage.addRequiredID(LiveValueMatrix::ID);
  pUsage.addRequiredID(MemAllocData::ID);
}

void LinearScanMemAlloc::print(OStream& pOS, const Module* pModule) const
{
  pOS << "=== LinearScanMemAlloc ===\n";
  m_MemAllocData->print(pOS, pModule);
}

LinearScanMemAlloc::AllocEntries
LinearScanMemAlloc::getSortedAllocatedRegions(const LIs& pLIs) const
{
  AllocEntries allocs;
  for (const LiveInterval* li : pLIs) {
    const Value* v = li->getValue();
    if (m_MemAllocData->hasAlloc(v))
      allocs.push_back(m_MemAllocData->getAlloc(v));
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