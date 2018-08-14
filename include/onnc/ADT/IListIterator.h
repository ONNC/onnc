//===- IListIterator.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ILIST_ITERATOR_H
#define ONNC_ADT_ILIST_ITERATOR_H
#include <onnc/ADT/IListNode.h>
#include <onnc/Support/DataTypes.h>
#include <cassert>
#include <iterator>

namespace onnc {

class IListIteratorBase
{
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

protected:
  IListNodeBase* m_pNodePtr;
  
public:
  IListIteratorBase();

  explicit IListIteratorBase(IListNodeBase* pNode);

  virtual ~IListIteratorBase() { }

  const IListNodeBase* getRawPtr() const { return m_pNodePtr; }

  IListNodeBase*       getRawPtr()       { return m_pNodePtr; }

protected:
  void advance();

  void retreat();
};

/** \class IListIterator
 *
 *  IListIterator is a bidirectional iterator.
 */
template<typename NodeType>
class IListIterator : public IListIteratorBase
{
public:
  typedef NodeType  value_type;
  typedef NodeType* pointer;
  typedef NodeType& reference;

public:
  IListIterator();

  explicit IListIterator(IListNodeBase* pNode);

  virtual ~IListIterator() { }

  template<typename NT>
  IListIterator(IListIterator<NT>& pRHS)
    : IListIterator(pRHS.getRawPtr()) { }

  operator pointer() const;

  reference operator* () const;
  pointer   operator->() const;

  IListIterator& operator++();
  IListIterator& operator--();
  IListIterator  operator++(int);
  IListIterator  operator--(int);

private:
  // IListIterator is a bidirectional iterator, but it has an implicit
  // conversion to pointer-type. Therefore, declare these functions
  // (but don't implement them) as private can help users catch
  // accidental mis-use.
  void operator+(difference_type) const;
  void operator-(difference_type) const;
  void operator+=(difference_type) const;
  void operator-=(difference_type) const;
  template<class T> void operator<(T) const;
  template<class T> void operator<=(T) const;
  template<class T> void operator>(T) const;
  template<class T> void operator>=(T) const;
  template<class T> void operator-(T) const;
};

//===----------------------------------------------------------------------===//
// IListIterator Member Functions
//===----------------------------------------------------------------------===//
template<typename NodeType>
IListIterator<NodeType>::IListIterator()
  : IListIteratorBase() {
}

template<typename NodeType>
IListIterator<NodeType>::IListIterator(IListNodeBase* pNode)
  : IListIteratorBase(pNode) {
}

template<typename NodeType>
IListIterator<NodeType>::operator pointer() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
typename IListIterator<NodeType>::reference
IListIterator<NodeType>::operator* () const
{
  assert(NULL != m_pNodePtr && "Can not dereference a NULL pointer");
  return *static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
typename IListIterator<NodeType>::pointer
IListIterator<NodeType>::operator->() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
IListIterator<NodeType>& IListIterator<NodeType>::operator++()
{
  advance();
  return *this;
}

template<typename NodeType>
IListIterator<NodeType>& IListIterator<NodeType>::operator--()
{
  retreat();
  return *this;
}

template<typename NodeType>
IListIterator<NodeType> IListIterator<NodeType>::operator++(int)
{
  IListIterator result(*this);
  advance();
  return result;
}

template<typename NodeType>
IListIterator<NodeType> IListIterator<NodeType>::operator--(int)
{
  IListIterator result(*this);
  retreat();
  return result;
}

} // namespace of onnc

#endif
