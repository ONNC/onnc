//===- TopologyIterator.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_TOPOLOGY_ITERATOR_H
#define ONNC_ADT_DIGRAPH_TOPOLOGY_ITERATOR_H
#include <onnc/ADT/Bits/PolicyNodeIterator.h>
#include <vector>
#include <deque>
#include <unordered_set>
#include <algorithm>

namespace onnc {
namespace digraph {

class TopoIteratorBase : public NodeIteratorBase
{
public:
  TopoIteratorBase();

  bool isEnd() const;

  void advance();

protected:
  typedef std::vector<NodeBase*> OrderList;
  typedef std::deque<std::pair<bool, NodeBase*> > Stack;
  typedef std::vector<NodeBase*> PostOrder;
  typedef std::unordered_set<NodeBase*> Visited;

protected:
  void InitOrderList(NodeBase* pNode);

  virtual void doPushAllChildren(NodeBase& pRoot, Visited& pV, Stack& pS) = 0;

protected:
  unsigned int m_Idx;
  OrderList m_OrderList;
};

class TopoIterator : public TopoIteratorBase
{
public:
  TopoIterator();

  TopoIterator(NodeBase* pNode);

  TopoIterator(const NodeBase* pNode);

protected:
  void doPushAllChildren(NodeBase& pRoot, Visited& pV, Stack& pS) override;
};

class RevTopoIterator : public TopoIteratorBase
{
public:
  RevTopoIterator();

  RevTopoIterator(NodeBase* pNode);

  RevTopoIterator(const NodeBase* pNode);

protected:
  void doPushAllChildren(NodeBase& pRoot, Visited& pV, Stack& pS) override;
};

} // namespace of digraph

template<typename NodeType, template<typename N = NodeType> class Traits>
using TopologyIterator = digraph::PolicyNodeIterator<digraph::TopoIterator, Traits<NodeType> >;

template<typename NodeType, template<typename N = NodeType> class Traits>
using RevTopologyIterator = digraph::PolicyNodeIterator<digraph::RevTopoIterator, Traits<NodeType> >;

} // namespace of onnc

#endif
