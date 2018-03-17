//===- TreeIterator.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_BITS_TREE_ITERATOR_H
#define ONNC_ADT_BITS_TREE_ITERATOR_H
#include <onnc/ADT/Bits/TreeNode.h>
#include <iterator>
#include <queue>
#include <stack>

namespace onnc {
namespace bintree {

/** \class TreeIteratorBase
 *  \brief TreeIteratorBase provides basic operators on a tree node
 */
class TreeIteratorBase
{
public:
  enum Direct {
    Leftward,
    Rightward
  };

public:
  TreeIteratorBase() : m_pNode(NULL) { }

  explicit TreeIteratorBase(NodeBase *X) : m_pNode(X) { }

  virtual ~TreeIteratorBase() { }

  template<size_t DIRECT>
  void move() { assert(false && "Illegal direction"); }

  template<size_t DIRECT>
  void hook(NodeBase* pOther) { assert(false && "Illegal direction"); }

  bool isRoot() const { return (m_pNode->right == m_pNode); }

  bool hasRightChild() const
  { return ((m_pNode->right) != (m_pNode->right->right)); }

  bool hasLeftChild() const
  { return ((m_pNode->left) != (m_pNode->left->right)); }

  bool operator==(const TreeIteratorBase& pOther) const
  { return this->m_pNode == pOther.m_pNode; }

  bool operator!=(const TreeIteratorBase& pOther) const
  { return !(*this == pOther); }

protected:
  NodeBase* m_pNode;
};

/// Partial specification for LeftWard
template<> inline
void TreeIteratorBase::move<TreeIteratorBase::Leftward>()
{
  this->m_pNode = this->m_pNode->left;
}

/// partial specification for rightward
template<> inline
void TreeIteratorBase::move<TreeIteratorBase::Rightward>()
{
  this->m_pNode = this->m_pNode->right;
}

/// Partial specification for LeftWard
template<> inline
void TreeIteratorBase::hook<TreeIteratorBase::Leftward>(NodeBase* pOther)
{
  this->m_pNode->left = pOther;
}

/// partial specification for rightward
template<> inline
void TreeIteratorBase::hook<TreeIteratorBase::Rightward>(NodeBase* pOther)
{
  this->m_pNode->right = pOther;
}

/** \class DFSIterator
 *  \brief DFSIterator traverses the tree by DFS order
 */
class DFSIterator : public TreeIteratorBase
{
public:
  DFSIterator() : TreeIteratorBase() { }

  DFSIterator(NodeBase *X)
    : TreeIteratorBase(X) {
    if (hasRightChild())
      m_Stack.push(m_pNode->right);
    if (hasLeftChild())
      m_Stack.push(m_pNode->left);
  }

  void advance() {
    if (m_Stack.empty()) { // reach the end
      m_pNode = m_pNode->right; // should be root
      return;
    }
    m_pNode = m_Stack.top();
    m_Stack.pop();
    if (hasRightChild())
      m_Stack.push(m_pNode->right);
    if (hasLeftChild())
      m_Stack.push(m_pNode->left);
  }

private:
    std::stack<NodeBase *> m_Stack;
};

class BFSIterator : public TreeIteratorBase
{
public:
  BFSIterator() : TreeIteratorBase() { }

  BFSIterator(NodeBase *X)
    : TreeIteratorBase(X) {
    if (hasRightChild())
      m_Queue.push(m_pNode->right);
    if (hasLeftChild())
      m_Queue.push(m_pNode->left);
  }

  void advance() {
    if (m_Queue.empty()) { // reach the end
      m_pNode = m_pNode->right; // should be root
      return;
    }
    m_pNode = m_Queue.front();
    m_Queue.pop();
    if (hasRightChild())
      m_Queue.push(m_pNode->right);
    if (hasLeftChild())
      m_Queue.push(m_pNode->left);
  }

private:
    std::queue<NodeBase *> m_Queue;
};

/** \class PolicyIterator
 *  \brief PolicyIterator provides a policy-based design iterator. Its
 *  behavios changes when the policy changes.
 */
template<class IteratorType, class Traits>
class PolicyIterator : public IteratorType
{
public:
  typedef Traits    traits;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef std::forward_iterator_tag   iterator_category;
  typedef typename traits::value_type value_type;
  typedef typename traits::pointer    pointer;
  typedef typename traits::reference  reference;
  typedef typename traits::template rebind<Node<value_type> >::other::value_type node_type;

public:
  PolicyIterator() : IteratorType() {}

  PolicyIterator(const PolicyIterator& X) : IteratorType(X.m_pNode) {}

  explicit PolicyIterator(NodeBase* X) : IteratorType(X) {}

  pointer operator*() const { return node()->data; }

  reference operator->() const { return *node()->data; }

  bool hasData() const {
    return (!IteratorType::isRoot() && (NULL != node()->data));
  }

  PolicyIterator& operator++() {
    IteratorType::advance();
    return *this;
  }

  PolicyIterator operator++(int) {
    PolicyIterator tmp = *this;
    IteratorType::advance();
    return tmp;
  }

private:
  node_type* node() const { return static_cast<node_type*>(IteratorType::m_pNode); }
  node_type* node()       { return static_cast<node_type*>(IteratorType::m_pNode); }
};

/** \class TreeIterator
 *  \brief TreeIterator provides full functions of binary tree's iterator.
 *
 *  TreeIterator is designed to compatible with STL iterators.
 *  TreeIterator is bi-directional. Incremental direction means to move
 *  rightward, and decremental direction is leftward.
 *
 *  @see TreeIteratorBase
 */
template<class Traits>
struct TreeIterator : public TreeIteratorBase
{
public:
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;

  typedef Traits                      traits;
  typedef typename traits::value_type value_type;
  typedef typename traits::pointer    pointer;
  typedef typename traits::reference  reference;

  typedef TreeIterator<traits> Self;
  typedef typename traits::template rebind<Node<value_type> >::other::value_type node_type;

public:
  TreeIterator() : TreeIteratorBase() { }

  TreeIterator(const TreeIterator& X) : TreeIteratorBase(X.m_pNode) { }

  explicit TreeIterator(NodeBase* X) : TreeIteratorBase(X) { }

  pointer operator*() const { return node()->data; }

  reference operator->() const { return *node()->data; }

  bool isRoot() const { return (m_pNode->right == m_pNode); }

  bool hasData() const { return (!isRoot() && (NULL != node()->data)); }

  Self& operator++() {
    this->move<Rightward>();
    return *this;
  }

  Self operator++(int) {
    Self tmp = *this;
    this->move<Rightward>();
    return tmp;
  }

  Self& operator--() {
    this->move<Leftward>();
    return *this;
  }

  Self operator--(int) {
    Self tmp = *this;
    this->move<Leftward>();
    return tmp;
  }

  node_type* node() const { return static_cast<node_type*>(m_pNode); }
  node_type* node()       { return static_cast<node_type*>(m_pNode); }
};

} // namespace of bintree
} // namespace of onnc

#endif
