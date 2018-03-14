//===- PassInfo.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassInfo.h>
#include <cassert>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PassInfo
//===----------------------------------------------------------------------===//
PassInfo::PassInfo(StringRef pName, Pass::AnalysisID pID)
  : m_Name(pName), m_PassID(pID),
    m_PassCtor(nullptr), m_PassTargetCtor(nullptr) {
}

PassInfo::PassInfo(StringRef pName, Pass::AnalysisID pID,
                   PassCtorFn pFn, PassTargetCtorFn pTrgFn)
  : m_Name(pName), m_PassID(pID), m_PassCtor(pFn), m_PassTargetCtor(pTrgFn) {
}

Pass *PassInfo::makePass(TargetBackend* pTB) const
{
  if (hasTargetCtor())
    return createPass(pTB);
  return createPass();
}

Pass *PassInfo::createPass() const
{
  assert(nullptr != m_PassCtor &&
         "Cannot call createPass on PassInfo without default ctor!");
  return m_PassCtor();
}

Pass *PassInfo::createPass(TargetBackend* pBackend) const
{
  assert(nullptr != m_PassTargetCtor &&
         "Cannot call createPass on PassInfo without target ctor!");
  return m_PassTargetCtor(pBackend);
}
