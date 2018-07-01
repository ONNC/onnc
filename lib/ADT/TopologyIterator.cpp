//===-- TopologyIterator.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/TopologyIterator.h>
#include <onnc/ADT/ArcIterator.h>
#include <deque>
#include <unordered_set>
#include <algorithm>

using namespace onnc;
using namespace onnc::digraph;

//===----------------------------------------------------------------------===//
// TopologyIterator
//===----------------------------------------------------------------------===//
TopoIterator::TopoIterator()
  : m_Idx(0), m_OrderList() {
}

TopoIterator::TopoIterator(NodeBase* pNode)
  : m_Idx(0), m_OrderList() {
  InitOrderList(pNode);
  setNode(m_OrderList[m_Idx]);
}

TopoIterator::TopoIterator(const NodeBase* pNode)
  : m_Idx(0), m_OrderList() {
  InitOrderList(const_cast<NodeBase*>(pNode));
  setNode(m_OrderList[m_Idx]);
}

bool TopoIterator::isEnd() const
{
  return (m_Idx == m_OrderList.size());
}

void TopoIterator::advance()
{
  ++m_Idx;
  setNode(m_OrderList[m_Idx]);
}

void TopoIterator::InitOrderList(NodeBase* pNode)
{
  typedef std::deque<std::pair<bool, NodeBase*> > Stack;
  typedef std::vector<NodeBase*> PostOrder;

  std::unordered_set<NodeBase*> visited;
  // true: parent, false: child
  Stack stack;
  PostOrder post_order;

  stack.push_back(std::make_pair(false, pNode));

  while (!stack.empty()) {
    std::pair<bool, NodeBase*> node = stack.back();
    stack.pop_back();

    if (node.first) { // get a parent
      if (post_order.end() == std::find(post_order.begin(), post_order.end(), node.second)) {
        post_order.push_back(node.second);
      }
    }
    else { // get a child

      // turn the child to parent
      visited.insert(node.second);
      stack.push_back(std::make_pair(true, node.second));

      // push all children
      ArcBase* iter = node.second->first_out;
      while (nullptr != iter) {
        // if not visited
        if (visited.end() == visited.find(iter->target)) {
          stack.push_back(std::make_pair(false, iter->target));
        }
        iter = iter->next_out;
      }
    }
  }

  PostOrder::reverse_iterator ele, eEnd = post_order.rend();
  for (ele = post_order.rbegin(); ele != eEnd; ++ele)
    m_OrderList.push_back(*ele);
}
