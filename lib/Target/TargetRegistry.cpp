//===-- TargetRegistry.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Support/ManagedStatic.h>

using namespace onnc;

static onnc::ManagedStatic<onnc::TargetRegistry::TargetList> s_TargetList;

//===----------------------------------------------------------------------===//
// onnc::TargetRegistry
//===----------------------------------------------------------------------===//
onnc::TargetRegistry::iterator TargetRegistry::Begin()
{
  return s_TargetList->begin();
}

onnc::TargetRegistry::iterator TargetRegistry::End()
{
  return s_TargetList->end();
}

size_t onnc::TargetRegistry::Size()
{
  return s_TargetList->size();
}

bool onnc::TargetRegistry::IsEmpty()
{
  return s_TargetList->empty();
}

void onnc::TargetRegistry::RegisterTarget(Target& pTarget,
                                    const char* pName,
                                    const char* pShortDesc,
                                    Target::QuadrupleMatchFnTy pArchMatchFn)
{
  pTarget.m_Name = pName;
  pTarget.m_ShortDesc = pShortDesc;
  pTarget.m_ArchMatchFn = pArchMatchFn;
  s_TargetList->push_back(&pTarget);
}
