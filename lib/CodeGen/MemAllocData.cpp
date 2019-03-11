//===- MemAllocData.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/MemAllocData.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>
#include <iomanip>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MemAllocData
//===----------------------------------------------------------------------===//
void MemAllocData::addAlloc(Value* pVal, const AllocEntry& pAlloc)
{
  assert(!hasAlloc(pVal) && "The value has been allocated.");
  m_ValToAllocEntry[pVal] = pAlloc;
}

MemAllocData::AllocEntry
MemAllocData::getAlloc(const Value* pVal) const
{
  auto it = m_ValToAllocEntry.find(const_cast<Value*>(pVal));
  assert(it != m_ValToAllocEntry.end() &&
         "Value has no memory allocation.");
  return it->second;
}

bool MemAllocData::hasAlloc(const Value* pVal) const
{
  auto it = m_ValToAllocEntry.find(const_cast<Value*>(pVal));
  return it != m_ValToAllocEntry.end();
}

void MemAllocData::print(OStream& pOS, const Module* pModule) const
{
  pOS << "=== MemAllocData ===\n";

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

  for (auto& it : m_ValToAllocEntry) {
    const Value* v = it.first;
    AllocEntry alloc = it.second;
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

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(MemAllocData, "MemAllocData")
}

ModulePass* onnc::CreateMemAllocDataPass()
{
  return new MemAllocData();
}
