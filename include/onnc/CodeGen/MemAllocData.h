//===- MemAllocData.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CODEGEN_MEM_ALLOC_DATA_H
#define ONNC_CODEGEN_MEM_ALLOC_DATA_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class MemAllocData
 *  \brief The pass is a data pass for saving memory allocation result.
 */
class MemAllocData : public CustomPass<MemAllocData>
{
public:
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

public:
  MemAllocData()
    : m_ValToAllocEntry() {
  }

  StringRef getPassName() const override { return "MemAllocData"; }

  // Data Pass. Do nothing in runOnModule.
  ReturnType runOnModule(Module &pModule) override { return kModuleNoChanged; }

  void addAlloc(Value* pVal, const AllocEntry& pAlloc);

  AllocEntry getAlloc(const Value* pVal) const;

  bool hasAlloc(const Value* pVal) const;

  void print(OStream& pOS, const Module* pModule) const override;

private:
  ValToAllocEntry m_ValToAllocEntry;
};

ModulePass* CreateMemAllocDataPass();

} // namespace onnc

#endif
