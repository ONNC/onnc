//===- IListIterator.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/IListIterator.h>
#include <cassert>

using namespace onnc;

//===----------------------------------------------------------------------===//
// IListIteratorBase
//===----------------------------------------------------------------------===//
IListIteratorBase::IListIteratorBase()
  : m_pNodePtr(NULL) {
}

IListIteratorBase::IListIteratorBase(IListNodeBase* pNode)
  : m_pNodePtr(pNode) {
}

void IListIteratorBase::advance()
{
  assert(NULL != m_pNodePtr && "trivial iterator can not advance");
  m_pNodePtr = m_pNodePtr->getNext();
}

void IListIteratorBase::retreat()
{
  assert(NULL != m_pNodePtr && "trivial iterator can not retreat");
  m_pNodePtr = m_pNodePtr->getPrev();
}
