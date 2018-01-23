//===- IListNode.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_ILIST_NODE_H
#define ONNC_ADT_ILIST_NODE_H
#include <stddef.h>
#include <cassert>

namespace onnc {

/** \class IListNodeBase
 *  \brief Base of IListNode.
 */
class IListNodeBase
{
public:
  IListNodeBase() : m_pPrev(NULL), m_pNext(NULL) { }

  IListNodeBase(IListNodeBase* pPrev, IListNodeBase* pNext)
  : m_pPrev(pPrev), m_pNext(pNext) { }

  virtual ~IListNodeBase() { }

  const IListNodeBase* getPrev() const { return m_pPrev; }
  IListNodeBase*       getPrev()       { return m_pPrev; }

  const IListNodeBase* getNext() const { return m_pNext; }
  IListNodeBase*       getNext()       { return m_pNext; }

  void setPrev(IListNodeBase *pPrev) { m_pPrev = pPrev; }
  void setNext(IListNodeBase *pNext) { m_pNext = pNext; }

private:
  IListNodeBase* m_pPrev;
  IListNodeBase* m_pNext;
};

/** \class IListNode
 *  \brief IListNode is the node of intrusive list.
 *
 *  IListNode::getPrevNode() and getNextNode() casts IListNodeBase to the given
 *  template argument NodeType.
 */
template<typename NodeType>
class IListNode : public IListNodeBase
{
public:
  typedef IListNodeBase Base;

public:
  IListNode() : IListNodeBase() { }

  IListNode(IListNode* pPrev, IListNode* pNext)
  : IListNodeBase(pPrev, pNext) { }

  const NodeType* getPrevNode() const;
  NodeType*       getPrevNode();

  const NodeType* getNextNode() const;
  NodeType*       getNextNode();
};

//===----------------------------------------------------------------------===//
// IListNode Member Functions
//===----------------------------------------------------------------------===//
template<typename NodeType>
const NodeType* IListNode<NodeType>::getPrevNode() const
{
  return static_cast<const NodeType*>(getPrev());
}

template<typename NodeType>
NodeType* IListNode<NodeType>::getPrevNode()
{
  return static_cast<NodeType*>(getPrev());
}

template<typename NodeType>
const NodeType* IListNode<NodeType>::getNextNode() const
{
  return static_cast<const NodeType*>(getNext());
}

template<typename NodeType>
NodeType* IListNode<NodeType>::getNextNode()
{
  return static_cast<NodeType*>(getNext());
}

} // namespace of onnc

#endif
