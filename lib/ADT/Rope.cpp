//===- Rope.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Rope.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/Path.h>
#include <sstream>
#include <cassert>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Rope
//===----------------------------------------------------------------------===//
Rope::Rope()
  : m_LHS(EmptyKind), m_RHS(EmptyKind) {
  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const char* pStr)
  : m_RHS(EmptyKind) {
  assert(NULL != pStr && "NULL is passed in Rope constructor");
  if ('\0' != pStr[0]) {
    m_LHS.data = (intptr_t)pStr;
    m_LHS.kind = CStringKind;
  }
  else
    m_LHS.kind = EmptyKind;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const std::string& pStr)
  : m_LHS(StdStringKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t) &pStr;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const StringRef& pStr)
  : m_LHS(StringRefKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t) &pStr;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const Path& pPath)
  : m_LHS(PathKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t) &pPath;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(char pVal)
  : m_LHS(CharKind), m_RHS(EmptyKind) {
  m_LHS.data = static_cast<intptr_t>(pVal);

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(signed char pValue)
  : m_LHS(CharKind), m_RHS(EmptyKind) {
  m_LHS.data = static_cast<intptr_t>(pValue);

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(unsigned char pValue)
  : m_LHS(CharKind), m_RHS(EmptyKind) {
  m_LHS.data = static_cast<intptr_t>(pValue);

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(int pValue)
  : m_LHS(IntKind), m_RHS(EmptyKind) {
  m_LHS.data = static_cast<intptr_t>(pValue);

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(unsigned int pValue)
  : m_LHS(UIntKind), m_RHS(EmptyKind) {
  m_LHS.data = static_cast<intptr_t>(pValue);

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const unsigned long& pValue)
  : m_LHS(ULongKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t)&pValue;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const long& pValue)
  : m_LHS(LongKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t)&pValue;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const unsigned long long& pValue)
  : m_LHS(ULongLongKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t)&pValue;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const long long& pValue)
  : m_LHS(LongLongKind), m_RHS(EmptyKind) {
  m_LHS.data = (intptr_t)&pValue;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const char* pLHS, const StringRef& pRHS)
  : m_LHS(CStringKind), m_RHS(StringRefKind) {
  m_LHS.data = (intptr_t)pLHS;
  m_RHS.data = (intptr_t)&pRHS;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const StringRef& pLHS, const char* pRHS)
  : m_LHS(StringRefKind), m_RHS(CStringKind) {
  m_LHS.data = (intptr_t)&pLHS;
  m_RHS.data = (intptr_t)pRHS;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(Kind pKind)
  : m_LHS(pKind), m_RHS(EmptyKind) {

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const Rope& pLHS, const Rope& pRHS)
  : m_LHS(RopeKind), m_RHS(RopeKind) {
  m_LHS.data = (intptr_t)&pLHS;
  m_RHS.data = (intptr_t)&pRHS;

  assert(isValid() && "Invalid rope!");
}

Rope::Rope(const Child& pLHS, const Child& pRHS)
  : m_LHS(pLHS), m_RHS(pRHS) {
}

Rope Rope::concat(const Rope& pSuffix) const
{
  // Concatenation with null is null.
  if (isNull() || pSuffix.isNull())
    return Rope(NullKind);

  // Concatenation with empty yields the other side.
  if (isEmpty())
    return pSuffix;
  if (pSuffix.isEmpty())
    return *this;

  // Otherwise we need to create a new node, taking care to fold in unary
  // twines.
  Child lhs(RopeKind), rhs(RopeKind);
  lhs.data = (intptr_t)this;
  rhs.data = (intptr_t)&pSuffix;

  if (isUnary()) {
    lhs = m_LHS;
  }
  if (pSuffix.isUnary()) {
    rhs = pSuffix.m_LHS;
  }

  return Rope(lhs, rhs);
}

std::string Rope::str() const
{
  std::stringstream ss;
  toStr(ss);
  return ss.str();
}

void Rope::toStr(std::ostream& pOS) const
{
  m_LHS.toStr(pOS);
  m_RHS.toStr(pOS);
}

//===----------------------------------------------------------------------===//
// Rope::Child
//===----------------------------------------------------------------------===//
void Rope::Child::toStr(std::ostream& pOS) const
{
  switch (getKind()) {
    case NullKind:
    case EmptyKind:
      return;
    case RopeKind:
      rope()->toStr(pOS);
      break;
    case CStringKind:
      pOS << (const char*)data;
      break;
    case StdStringKind:
      pOS << *((std::string*)data);
      break;
    case StringRefKind:
      pOS << ((StringRef*)data)->str();
      break;
    case PathKind:
      pOS << ((Path*)data)->native();
      break;
    case CharKind:
      pOS << (char)data;
      break;
    case IntKind:
      pOS << (int)data;
      break;
    case UIntKind:
      pOS << (unsigned int)data;
      break;
    case ULongKind:
      pOS << *((unsigned long*)data);
      break;
    case LongKind:
      pOS << *((long*)data);
      break;
    case ULongLongKind:
      pOS << *((unsigned long long*)data);
      break;
    case LongLongKind:
      pOS << *((long long*)data);
      break;
  } // switch
}

