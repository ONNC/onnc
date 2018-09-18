//===- LinearScanMemAlloc.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_LINEAR_SCAN_MEM_ALLOC_H
#define ONNC_CODEGEN_LINEAR_SCAN_MEM_ALLOC_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

class LiveInterval;
class LiveIntervals;
class LiveValueMatrix;
class TargetBackend;
class TargetMemInfo;

/** \class LinearScanMemAlloc
 *  \brief Linear memory allocation for each value considering value's liveness.
 */
class LinearScanMemAlloc : public ModulePass
{
public:
  static char ID;

  struct AllocEntry
  {
    uint64_t startAddr, size;
    AllocEntry(uint64_t pStartAddr, uint64_t pSize)
      : startAddr(pStartAddr), size(pSize) {
    }

    AllocEntry()
      : startAddr(0), size(0) {
    }

    bool overlap(const AllocEntry& pOther)
    {
      uint64_t myEnd = startAddr + size,
               otherEnd = pOther.startAddr + pOther.size;
      return !(myEnd <= pOther.startAddr || otherEnd <= startAddr);
    }
  };

  typedef std::unordered_map<Value*, AllocEntry> ValToAllocEntry;

  typedef std::vector<AllocEntry> AllocEntries;

  typedef std::vector<LiveInterval*> LIs;

public:
  LinearScanMemAlloc(TargetBackend* pTarget = nullptr);

  StringRef getPassName() const override { return "LinearScanMemAlloc"; }

  Pass::ReturnType runOnModule(Module &pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  AllocEntry getAlloc(const Value* pVal) const;

  bool hasAlloc(const Value* pVal) const;

  void print(OStream& pOS, const Module* pModule) const override;

private:
  AllocEntries getSortedAllocatedRegions(const LIs& pLIs) const;

  AllocEntry getAnEmptyRegion(const AllocEntries& pAllocs,
                              uint64_t pRequiredSize,
                              uint64_t pAlignment) const;

private:
  ValToAllocEntry m_ValToAllocEntry;
  LiveIntervals* m_LIPass;
  LiveValueMatrix* m_LiveMatPass;
  TargetMemInfo* m_TMI;
};

ModulePass* CreateLinearScanMemAllocPass(TargetBackend* pTB);

} // namespace onnc

#endif