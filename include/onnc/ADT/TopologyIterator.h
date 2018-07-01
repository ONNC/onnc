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

namespace onnc {
namespace digraph {

class TopoIterator : public NodeIteratorBase
{
public:
  TopoIterator();

  TopoIterator(NodeBase* pNode);

  TopoIterator(const NodeBase* pNode);

  bool isEnd() const;

  void advance();

private:
  typedef std::vector<NodeBase*> OrderList;

private:
  void InitOrderList(NodeBase* pNode);

private:
  unsigned int m_Idx;
  OrderList m_OrderList;
};

} // namespace of digraph

template<typename NodeType, template<typename N = NodeType> class Traits>
using TopologyIterator = digraph::PolicyNodeIterator<digraph::TopoIterator, Traits<NodeType> >;

} // namespace of onnc

#endif
