//===- PolicyNodeIterator.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_POLICY_NODE_ITERATOR_H
#define ONNC_ADT_DIGRAPH_POLICY_NODE_ITERATOR_H
#include <onnc/ADT/NodeIterator.h>
#include <set>
#include <queue>
#include <stack>

namespace onnc {
namespace digraph {

class BFSIterator : public NodeIteratorBase
{
public:
  BFSIterator();

  BFSIterator(NodeBase* pNode);

  bool isEnd() const;

  void advance();

private:
  typedef std::queue<NodeBase*> Queue;
  typedef std::set<NodeBase*> Visited;

private:
  Queue m_Queue;
  Visited m_Visited;
};

class DFSIterator : public NodeIteratorBase
{
public:
  DFSIterator();

  DFSIterator(NodeBase* pNode);

  bool isEnd() const;

  void advance();

private:
  typedef std::stack<NodeBase*> Stack;
  typedef std::set<NodeBase*> Visited;

private:
  Stack m_Stack;
  Visited m_Visited;
};

template<typename IteratorType, typename Traits>
class PolicyNodeIterator : public IteratorType
{
public:
  typedef Traits    traits;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef std::forward_iterator_tag   iterator_category;
  typedef typename traits::value_type value_type;
  typedef typename traits::pointer    pointer;
  typedef typename traits::reference  reference;

public:
  PolicyNodeIterator()
    : IteratorType() { }

  PolicyNodeIterator(value_type* pNode)
    : IteratorType(pNode) {
  }

  ~PolicyNodeIterator() { }

  operator pointer() const     { return node(); }
  pointer   operator->() const { return node(); }
  reference operator*()  const { return *node(); }

  PolicyNodeIterator& next() {
    IteratorType::advance();
    return *this;
  }

  pointer node() const { return static_cast<pointer>(IteratorType::m_pNodePtr); }
};

template<typename IteratorType, typename Traits> inline bool
operator==(const PolicyNodeIterator<IteratorType, Traits>& pX,
           const PolicyNodeIterator<IteratorType, Traits>& pY) {
  return (pX.node() == pY.node()); 
}

} // namespace of digraph
} // namespace of onnc

#endif
