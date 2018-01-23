//===- IList.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/IList.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// IListBase
//===----------------------------------------------------------------------===//
IListBase::IListBase()
  : m_pHead(NULL) {
  m_pHead = new Sentinel(m_pHead);
  m_pHead->setPrev(m_pHead);
  m_pHead->setNext(m_pHead);
}

IListBase::~IListBase()
{
  delete getSentinel();
}

IListBase::size_type IListBase::size() const
{
  size_type counter = 0;
  IListNodeBase* cur = m_pHead;
  while (!isSentinel(*cur)) {
    ++counter;
    cur = cur->getNext();
  }
  return counter;
}

bool IListBase::empty() const
{
  return isSentinel(*m_pHead);
}

bool IListBase::isSentinel(const IListNodeBase& pNode)
{
  return (&pNode == pNode.getNext());
}

const IListNodeBase* IListBase::getSentinel() const
{
  return head()->getPrev();
}

IListNodeBase* IListBase::getSentinel()
{
  return head()->getPrev();
}

void IListBase::swap(IListBase& pOther)
{
  std::swap(m_pHead, pOther.m_pHead);
}

void IListBase::doInsert(IListNodeBase& pWhere, IListNodeBase& pNew)
{
  IListNodeBase* cur = &pWhere;
  IListNodeBase* prev = cur->getPrev();

  pNew.setNext(cur);
  pNew.setPrev(prev);

  if (!isSentinel(*prev))
    prev->setNext(&pNew);
  else
    setHead(&pNew);

  cur->setPrev(&pNew);
}

void IListBase::remove(IListNodeBase& pWhere)
{
  IListNodeBase* cur = &pWhere;
  IListNodeBase* prev = cur->getPrev();
  IListNodeBase* next = cur->getNext();

  if (!isSentinel(*prev)) {
    prev->setNext(next);
  }
  else { //< that is, cur is head
    ((Sentinel*)prev)->setHead(*next);
  }

  next->setPrev(prev);

  cur->setPrev(NULL);
  cur->setNext(NULL);
}

void IListBase::doReset()
{
  if (empty())
    return;

  IListNodeBase* sentinel = getSentinel();
  sentinel->setPrev(sentinel);
  m_pHead = sentinel;
}

void IListBase::doConcatenate(IListBase& pOther)
{
  if (pOther.empty() || this == &pOther)
    return;

  IListNodeBase* first = pOther.head();
  IListNodeBase* last  = pOther.tail();

  pOther.doReset();

  IListNodeBase* prev = tail();
  IListNodeBase* next = getSentinel();

  first->setPrev(prev);
  last->setNext(next);
  next->setPrev(last);
  if (prev == next) // prev == next == sentinel
    setHead(first);
  else
    prev->setNext(first);
}
