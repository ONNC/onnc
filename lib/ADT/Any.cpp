//===-- Any.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Any.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any
//===----------------------------------------------------------------------===//
Any::Any()
  : m_pContent(NULL) {
}

Any::Any(const Any& pCopy)
  : m_pContent((NULL == pCopy.m_pContent)? NULL: pCopy.m_pContent->clone()) {
}

Any::~Any()
{
  delete m_pContent;
}

Any& Any::swap(Any& pRHS)
{
  std::swap(m_pContent, pRHS.m_pContent);
  return *this;
}

Any& Any::operator=(const Any& pRHS)
{
  Any(pRHS).swap(*this);
  return *this;
}

bool Any::empty() const
{
  return (NULL == m_pContent);
}

void Any::clear()
{
  Any().swap(*this);
}

void Any::print(std::ostream& pOS) const
{
  if (!empty()) {
    m_pContent->print(pOS);
  }
}

Any* Any::clone() const
{
  return new Any(*this);
}
