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
  NodeIteratorBase()
    : m_pNodePtr(nullptr) {
  }

  explicit NodeIteratorBase(digraph::NodeBase* pPtr)
    : m_pNodePtr(pPtr) {
  }

  virtual ~NodeIteratorBase() { m_pNodePtr = nullptr; }

protected:
  void setNode(digraph::NodeBase* pPtr) { m_pNodePtr = pPtr; }

protected:
  digraph::NodeBase* m_pNodePtr;
};

template<typename NodeType, template<typename N = NodeType> class Traits>
class NodeIterator : public NodeIteratorBase
{
public:
  typedef Traits<NodeType>    traits;
  typedef size_t              size_type;
  typedef ptrdiff_t           difference_type;
  typedef typename traits::value_type value_type;
  typedef typename traits::pointer    pointer;
  typedef typename traits::reference  reference;

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
  pointer node();

  NodeIterator& operator++();
  NodeIterator& operator--();

  NodeIterator  operator++(int);
  NodeIterator  operator--(int);
};

template<typename NodeType, template<typename N = NodeType> class Traits>
inline bool operator==(const NodeIterator<NodeType, Traits>& pX,
           const NodeIterator<NodeType, Traits>& pY) {
  return (pX.node() == pY.node()); 
}

//===----------------------------------------------------------------------===//
// Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits>::NodeIterator()
  : NodeIteratorBase() {
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits>::NodeIterator(NodeType* pNode)
  : NodeIteratorBase(pNode) {
}

template<typename NodeType, template<typename N = NodeType> class Traits>
bool NodeIterator<NodeType, Traits>::isEnd() const
{
  return (nullptr == m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
bool NodeIterator<NodeType, Traits>::hasNext() const
{
  return (nullptr != m_pNodePtr->next);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
bool NodeIterator<NodeType, Traits>::hasPrev() const
{
  return (nullptr != m_pNodePtr->prev);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits>::operator pointer() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
typename NodeIterator<NodeType, Traits>::pointer
NodeIterator<NodeType, Traits>::operator->() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
typename NodeIterator<NodeType, Traits>::reference
NodeIterator<NodeType, Traits>::operator*() const
{
  return *static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
typename NodeIterator<NodeType, Traits>::pointer
NodeIterator<NodeType, Traits>::node() const
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
typename NodeIterator<NodeType, Traits>::pointer
NodeIterator<NodeType, Traits>::node()
{
  return static_cast<pointer>(m_pNodePtr);
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits>& NodeIterator<NodeType, Traits>::operator++()
{
  m_pNodePtr = m_pNodePtr->next;
  return *this;
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits>& NodeIterator<NodeType, Traits>::operator--()
{
  m_pNodePtr = m_pNodePtr->prev;
  return *this;
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits> NodeIterator<NodeType, Traits>::operator++(int)
{
  return NodeIterator(node()->getNextNode());
}

template<typename NodeType, template<typename N = NodeType> class Traits>
NodeIterator<NodeType, Traits> NodeIterator<NodeType, Traits>::operator--(int)
{
  return NodeIterator(node()->getPrevNode());
}

} // namespace of onnc

#endif
