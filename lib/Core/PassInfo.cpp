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
  : m_Name(pName), m_PassID(pID), m_PassCtor(nullptr) {
}

PassInfo::PassInfo(StringRef pName, Pass::AnalysisID pID, PassCtorFn pFn)
  : m_Name(pName), m_PassID(pID), m_PassCtor(pFn) {
}

Pass *PassInfo::createPass() const
{
  assert(nullptr != m_PassCtor &&
         "Cannot call createPass on PassInfo without default ctor!");
    return m_PassCtor();
}
