//===- ArcIterator.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_INPUT_OUTPUT_ARC_ITERATOR_H
#define ONNC_ADT_DIGRAPH_INPUT_OUTPUT_ARC_ITERATOR_H
#include <onnc/ADT/Bits/DigraphArc.h>

namespace onnc {

template<typename ArcType>
class ArcIteratorBase
{
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef ArcType  value_type;
  typedef ArcType& reference;
  typedef ArcType* pointer;

protected:
  digraph::ArcBase* m_pArcPtr;

public:
  ArcIteratorBase()
    : m_pArcPtr(nullptr) {
  }

  explicit ArcIteratorBase(digraph::ArcBase* pPtr)
    : m_pArcPtr(pPtr) {
  }

  virtual ~ArcIteratorBase() { m_pArcPtr = nullptr; }

  bool isEnd() const   { return (nullptr == m_pArcPtr); }

  operator pointer() const { return static_cast<pointer>(m_pArcPtr); }

  pointer   operator->() const { return static_cast<pointer>(m_pArcPtr); }

  reference operator*()  const { return *static_cast<pointer>(m_pArcPtr); }

  pointer arc() const { return static_cast<pointer>(m_pArcPtr); }
};

template<typename NodeType, typename ArcType>
class InArcIterator : public ArcIteratorBase<ArcType>
{
public:
  InArcIterator();

  InArcIterator(NodeType& pArc);

  ~InArcIterator() { }

  bool hasNext() const {
    return (nullptr != ArcIteratorBase<ArcType>::m_pArcPtr->next_in);
  }

  bool hasPrev() const {
    return (nullptr != ArcIteratorBase<ArcType>::m_pArcPtr->prev_in);
  }

  InArcIterator& operator++();
  InArcIterator& operator--();

  InArcIterator  operator++(int);
  InArcIterator  operator--(int);
};

template<typename NodeType, typename ArcType>
class OutArcIterator : public ArcIteratorBase<ArcType>
{
public:
  OutArcIterator();

  OutArcIterator(NodeType& pNode);

  ~OutArcIterator() { }

  bool hasNext() const {
    return (nullptr != ArcIteratorBase<ArcType>::m_pArcPtr->next_out);
  }

  bool hasPrev() const {
    return (nullptr != ArcIteratorBase<ArcType>::m_pArcPtr->prev_out);
  }

  OutArcIterator& operator++();
  OutArcIterator& operator--();

  OutArcIterator  operator++(int);
  OutArcIterator  operator--(int);
};

template<typename NodeType, typename ArcType> inline bool
operator==(const InArcIterator<NodeType, ArcType>& pX,
           const InArcIterator<NodeType, ArcType>& pY) {
  return (pX.arc() == pY.arc()); 
}

template<typename NodeType, typename ArcType> inline bool
operator==(const OutArcIterator<NodeType, ArcType>& pX,
           const OutArcIterator<NodeType, ArcType>& pY) {
  return (pX.arc() == pY.arc()); 
}

//===----------------------------------------------------------------------===//
// InArcIterator Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType>::InArcIterator()
  : ArcIteratorBase<ArcType>() {
}

template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType>::InArcIterator(NodeType& pNode)
  : ArcIteratorBase<ArcType>(pNode.getFirstInArc()) {
}

template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType>& InArcIterator<NodeType, ArcType>::operator++()
{
  ArcIteratorBase<ArcType>::m_pArcPtr = ArcIteratorBase<ArcType>::arc()->getNextIn();
  return *this;
}

template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType>& InArcIterator<NodeType, ArcType>::operator--()
{
  ArcIteratorBase<ArcType>::m_pArcPtr = ArcIteratorBase<ArcType>::arc()->getPrevIn();
  return *this;
}

template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType> InArcIterator<NodeType, ArcType>::operator++(int)
{
  return InArcIterator(ArcIteratorBase<ArcType>::arc()->getNextIn());
}

template<typename NodeType, typename ArcType>
InArcIterator<NodeType, ArcType> InArcIterator<NodeType, ArcType>::operator--(int)
{
  return InArcIterator(ArcIteratorBase<ArcType>::arc()->getPrevIn());
}

//===----------------------------------------------------------------------===//
// OutArcIterator Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType>::OutArcIterator()
  : ArcIteratorBase<ArcType>() {
}

template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType>::OutArcIterator(NodeType& pNode)
  : ArcIteratorBase<ArcType>(pNode.getFirstOutArc()) {
}

template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType>& OutArcIterator<NodeType, ArcType>::operator++()
{
  ArcIteratorBase<ArcType>::m_pArcPtr = ArcIteratorBase<ArcType>::arc()->getNextOut();
  return *this;
}

template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType>& OutArcIterator<NodeType, ArcType>::operator--()
{
  ArcIteratorBase<ArcType>::m_pArcPtr = ArcIteratorBase<ArcType>::arc()->getPrevOut();
  return *this;
}

template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType> OutArcIterator<NodeType, ArcType>::operator++(int)
{
  return OutArcIterator(ArcIteratorBase<ArcType>::arc()->getNextOut());
}

template<typename NodeType, typename ArcType>
OutArcIterator<NodeType, ArcType> OutArcIterator<NodeType, ArcType>::operator--(int)
{
  return OutArcIterator(ArcIteratorBase<ArcType>::arc()->getPrevOut());
}

} // namespace of onnc

#endif
