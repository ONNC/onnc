//===- CArguRef.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/CArguRef.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CArguRef
//===----------------------------------------------------------------------===//
CArguRef::CArguRef(int pArgc, const char* const* pArgv)
  : m_Argus() {
  for (int i = 0; i < pArgc; ++i)
    m_Argus.push_back(pArgv[i]);
}

CArguRef::CArguRef(const CArgu& pCArgu)
  : m_Argus() {
  CArgu::const_iterator argu, aEnd = pCArgu.end();
  for (argu = pCArgu.begin(); argu != aEnd; ++argu) {
    m_Argus.push_back(*argu);
  }
}
