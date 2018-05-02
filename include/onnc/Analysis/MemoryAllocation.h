//===- MemoryAllocation.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MEMORY_ALLOCATION_H
#define ONNC_MEMORY_ALLOCATION_H
#include <onnc/Core/ModulePass.h>
#include <onnx/common/ir.h>
#include <vector>

namespace onnc {

class LiveInterval;

struct MemAllocEntry {
public:
  MemAllocEntry(size_t pStartAddr, size_t pSize, const LiveInterval& pIntrvl)
    : startAddr(pStartAddr), size(pSize), liveIntrvl(pIntrvl) {
  }

  size_t startAddr, size;
  const LiveInterval& liveIntrvl;
};

using MemAllocList = std::vector<MemAllocEntry*>;

/** \class MemoryAllocation
 *  Perform memory allocation and generate allocation map.
 */
class MemoryAllocation : public ModulePass
{
public:
  static char ID;

  virtual ~MemoryAllocation();

public:
  MemoryAllocation();

  bool runOnModule(Module& pModule) override;

  void getAnalysisUsage(AnalysisUsage& pUsage) const override;

  void print(std::ostream& pOS) const;

private:
  /// delete MemAllocEntry in m_MemAllocList
  void clear();

private:
  MemAllocList m_MemAllocList;
};

MemoryAllocation* CreateMemoryAllocationPass();

} // namespace of onnc

#endif
