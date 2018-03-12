//===- PassRegistry.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassRegistry.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static onnc::ManagedStatic<PassRegistry> g_PassRegistryObj;

PassRegistry* GetPassRegistry()
{
  return &*g_PassRegistryObj;
}

//===----------------------------------------------------------------------===//
// PassRegistry
//===----------------------------------------------------------------------===//
PassRegistry::~PassRegistry()
{
  clear();
}

const PassInfo* PassRegistry::getPassInfo(Pass::AnalysisID pID) const
{
  MapType::const_iterator entry = m_Map.find(pID);
  if (m_Map.end() == entry) // not found
    return nullptr;
  return entry->second;
}

void PassRegistry::registerPass(const PassInfo& pInfo)
{
  std::pair<Pass::AnalysisID, const PassInfo*> value(pInfo.getPassID(), &pInfo);
  std::pair<MapType::iterator,bool> result = m_Map.insert(value);
  if (false == result.second) { // not inserted
    error(pass_registered) << pInfo.getPassName();
  }
}

void PassRegistry::clear()
{
  MapType::iterator entry, eEnd = m_Map.end();
  for (entry = m_Map.begin(); entry != eEnd; ++entry) {
    delete entry->second;
  }
  m_Map.clear();
}

bool PassRegistry::isEmpty() const
{
  return m_Map.empty();
}

unsigned int PassRegistry::numOfPasses() const
{
  return m_Map.size();
}
