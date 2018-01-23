//===- IList.tcc ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// IList Member Functions
//===----------------------------------------------------------------------===//
template<typename NodeType>
typename IList<NodeType>::iterator IList<NodeType>::begin()
{
  return iterator(m_pHead);
}

template<typename NodeType>
typename IList<NodeType>::const_iterator IList<NodeType>::begin() const
{
  return const_iterator(m_pHead);
}

template<typename NodeType>
typename IList<NodeType>::iterator IList<NodeType>::end()
{
  return iterator(m_pHead->getPrev());
}

template<typename NodeType>
typename IList<NodeType>::const_iterator IList<NodeType>::end() const
{
  return const_iterator(m_pHead->getPrev());
}

template<typename NodeType>
typename IList<NodeType>::const_reverse_iterator IList<NodeType>::rbegin() const
{
  return const_reverse_iterator(end());
}

template<typename NodeType>
typename IList<NodeType>::reverse_iterator IList<NodeType>::rbegin()
{
  return reverse_iterator(end());
}

template<typename NodeType>
typename IList<NodeType>::const_reverse_iterator IList<NodeType>::rend()   const
{
  return const_reverse_iterator(begin());
}

template<typename NodeType>
typename IList<NodeType>::reverse_iterator IList<NodeType>::rend()
{
  return reverse_iterator(begin());
}

template<typename NodeType>
typename IList<NodeType>::const_reference IList<NodeType>::front() const
{
  assert(!empty() && "Calling front() on empty list!");
  return *static_cast<const_pointer>(head());
}

template<typename NodeType>
typename IList<NodeType>::reference IList<NodeType>::front()
{
  assert(!empty() && "Calling front() on empty list!");
  return *static_cast<pointer>(head());
}

template<typename NodeType>
typename IList<NodeType>::const_reference IList<NodeType>::back() const
{
  assert(!empty() && "Calling back() on empty list!");
  return *static_cast<const_pointer>(getSentinel()->getPrev());
}

template<typename NodeType>
typename IList<NodeType>::reference IList<NodeType>::back()
{
  assert(!empty() && "Calling back() on empty list!");
  return *static_cast<pointer>(getSentinel()->getPrev());
}

template<typename NodeType> typename IList<NodeType>::iterator
IList<NodeType>::insert(iterator pWhere, NodeType* pNew)
{
  doInsert(*pWhere.getRawPtr(), *pNew);
  return iterator(pNew);
}

template<typename NodeType> typename IList<NodeType>::iterator
IList<NodeType>::erase(iterator pWhere)
{
  assert(pWhere != end() && "Cannot remove end()");
  IListNodeBase* cur = pWhere.getRawPtr();
  IListNodeBase* next = pWhere.getRawPtr()->getNext();
  remove(*cur);
  return iterator(next);
}

template<typename NodeType>
void IList<NodeType>::push_front(NodeType* pValue)
{
  insert(this->begin(), pValue);
}

template<typename NodeType>
void IList<NodeType>::pop_front()
{
  assert(!empty() && "pop_front() on an empty list");
  erase(begin());
}

template<typename NodeType>
void IList<NodeType>::push_back(NodeType* pValue)
{
  insert(this->end(), pValue);
}

template<typename NodeType>
void IList<NodeType>::pop_back()
{
  assert(!empty() && "pop_back() on an empty list");
  iterator it = end();
  erase(--it);
}

template<typename NodeType>
void IList<NodeType>::concat(IList& pOther)
{
  doConcatenate(pOther);
  assert(pOther.empty());
}

template<typename NodeType>
typename IList<NodeType>::iterator
IList<NodeType>::erase(iterator pFirst, iterator pLast)
{
  while (pFirst != pLast) {
    pFirst = erase(pFirst);
  }
  return pLast;
}

template<typename NodeType>
void IList<NodeType>::clear()
{
  erase(begin(), end());
}

/// give up the control of all elements.
template<typename NodeType>
void IList<NodeType>::reset()
{
  doReset();
}
