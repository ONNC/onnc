//===- Notation.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/JSON/Notation.h>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// Notation 
//===----------------------------------------------------------------------===//
Notation::Notation()
  : m_Type(UNDEF) {
}

Notation::Notation(Type pType)
  : m_Type(pType) {
}

Notation::Notation(const Notation& pCopy)
  : m_Type(pCopy.m_Type) {
}

Notation::Notation(Notation&& pOther)
  : m_Type(UNDEF) {
  m_Type = pOther.m_Type;
  pOther.m_Type = UNDEF;
}

Notation& Notation::operator=(const Notation& pCopy)
{
  m_Type = pCopy.m_Type;
  return *this;
}

Notation& Notation::operator=(Notation&& pOther)
{
  if (this != &pOther) {
    m_Type = pOther.m_Type;
    pOther.m_Type = UNDEF;
  }
  return *this;
}
