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

class TopoAlgoBase : public NodeIteratorBase
{
public:
  TopoAlgoBase();

  bool isEnd() const;

  void advance();

protected:
  typedef digraph::NodeBase NodeType;
  typedef std::vector<NodeType*> OrderList;
  typedef std::vector<NodeType*> PostOrder;
  typedef std::unordered_set<NodeType*> Visited;

protected:
  unsigned int m_Idx;
  OrderList m_OrderList;
};

class TopoDFSAlgoBase : public TopoAlgoBase
{
public:
  TopoDFSAlgoBase();

protected:
  typedef std::deque<std::pair<bool, NodeType*> > Stack;

protected:
  void InitOrderList(NodeType* pNode);

  virtual void doPushAllChildren(NodeType& pRoot, Visited& pV, Stack& pS) = 0;
};

class TopoDFSAlgo : public TopoDFSAlgoBase
{
public:
  TopoDFSAlgo();

  TopoDFSAlgo(NodeType* pNode);

  TopoDFSAlgo(const NodeType* pNode);

protected:
  void doPushAllChildren(NodeType& pRoot, Visited& pV, Stack& pS) override;
};

class RevTopoDFSAlgo : public TopoDFSAlgoBase
{
public:
  RevTopoDFSAlgo();

  RevTopoDFSAlgo(NodeType* pNode);

  RevTopoDFSAlgo(const NodeType* pNode);

protected:
  void doPushAllChildren(NodeType& pRoot, Visited& pV, Stack& pS) override;
};

template<typename Algo, typename NodeType, template<typename N = NodeType> class Traits>
using TopologyIterator = digraph::PolicyNodeIterator<Algo, Traits<NodeType> >;

} // namespace of onnc

#endif
