//===- NodeIterator.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_NODE_ITERATOR_H
#define ONNC_ADT_DIGRAPH_NODE_ITERATOR_H
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/Support/DataTypes.h>

namespace onnc {

class NodeIteratorBase
{
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

protected:
  digraph::NodeBase* m_pNodePtr;

public:
  NodeIteratorBase()
    : m_pNodePtr(nullptr) {
  }

  explicit NodeIteratorBase(digraph::NodeBase* pPtr)
    : m_pNodePtr(pPtr) {
  }

  virtual ~NodeIteratorBase() { m_pNodePtr = nullptr; }

protected:
  void setNode(digraph::NodeBase* pPtr) { m_pNodePtr = pPtr; }
};

template<typename NodeType>
class NodeIterator : public NodeIteratorBase
{
public:
  typedef NodeType  value_type;
  typedef NodeType& reference;
  typedef NodeType* pointer;

public:
  NodeIterator();

  NodeIterator(NodeType* pNode);

  ~NodeIterator() { }

  bool isEnd() const;

  bool hasNext() const;

  bool hasPrev() const;

  operator pointer() const;

  pointer   operator->() const;
  reference operator*()  const;
  pointer node() const;

  NodeIterator& operator++();
  NodeIterator& operator--();

  NodeIterator  operator++(int);
  NodeIterator  operator--(int);
};

template<typename NodeType> inline bool
operator==(const NodeIterator<NodeType>& pX, const NodeIterator<NodeType>& pY) {
  return (pX.node() == pY.node()); 
}

//===----------------------------------------------------------------------===//
// Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType>
NodeIterator<NodeType>::NodeIterator()
  : NodeIteratorBase() {
}

template<typename NodeType>
NodeIterator<NodeType>::NodeIterator(NodeType* pNode)
  : NodeIteratorBase(pNode) {
}

template<typename NodeType>
bool NodeIterator<NodeType>::isEnd() const
{
  return (nullptr == m_pNodePtr);
}

template<typename NodeType>
bool NodeIterator<NodeType>::hasNext() const
{
  return (nullptr != m_pNodePtr->next);
}

template<typename NodeType>
bool NodeIterator<NodeType>::hasPrev() const
{
  return (nullptr != m_pNodePtr->prev);
}

template<typename NodeType>
NodeIterator<NodeType>::operator pointer() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
typename NodeIterator<NodeType>::pointer NodeIterator<NodeType>::operator->() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
typename NodeIterator<NodeType>::reference NodeIterator<NodeType>::operator*() const
{
  return *static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
typename NodeIterator<NodeType>::pointer NodeIterator<NodeType>::node() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType>
NodeIterator<NodeType>& NodeIterator<NodeType>::operator++()
{
  m_pNodePtr = node()->getNextNode();
  return *this;
}

template<typename NodeType>
NodeIterator<NodeType>& NodeIterator<NodeType>::operator--()
{
  m_pNodePtr = node()->getPrevNode();
  return *this;
}

template<typename NodeType>
NodeIterator<NodeType> NodeIterator<NodeType>::operator++(int)
{
  return NodeIterator(node()->getNextNode());
}

template<typename NodeType>
NodeIterator<NodeType> NodeIterator<NodeType>::operator--(int)
{
  return NodeIterator(node()->getPrevNode());
}

} // namespace of onnc

#endif
