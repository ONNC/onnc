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
// TopoIteratorBase
//===----------------------------------------------------------------------===//
TopoIteratorBase::TopoIteratorBase()
  : m_Idx(0), m_OrderList() {
}

bool TopoIteratorBase::isEnd() const
{
  return (m_Idx == m_OrderList.size());
}

void TopoIteratorBase::advance()
{
  ++m_Idx;
  setNode(m_OrderList[m_Idx]);
}

void TopoIteratorBase::InitOrderList(NodeBase* pNode)
{
  Visited visited;
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
      doPushAllChildren(*node.second, visited, stack);

    }
  }

  PostOrder::reverse_iterator ele, eEnd = post_order.rend();
  for (ele = post_order.rbegin(); ele != eEnd; ++ele)
    m_OrderList.push_back(*ele);
}

//===----------------------------------------------------------------------===//
// TopoIterator
//===----------------------------------------------------------------------===//
TopoIterator::TopoIterator()
  : TopoIteratorBase() {
}

TopoIterator::TopoIterator(NodeBase* pNode)
  : TopoIteratorBase() {
  InitOrderList(pNode);
  setNode(m_OrderList[m_Idx]);
}

TopoIterator::TopoIterator(const NodeBase* pNode)
  : TopoIteratorBase() {
  InitOrderList(const_cast<NodeBase*>(pNode));
  setNode(m_OrderList[m_Idx]);
}

void TopoIterator::doPushAllChildren(NodeBase& pRoot, Visited& pV, Stack& pS)
{
  ArcBase* iter = pRoot.first_out;
  while (nullptr != iter) {
    // if not visited
    if (pV.end() == pV.find(iter->target)) {
      pS.push_back(std::make_pair(false, iter->target));
    }
    iter = iter->next_out;
  }
}

//===----------------------------------------------------------------------===//
// RevTopoIterator
//===----------------------------------------------------------------------===//
RevTopoIterator::RevTopoIterator()
  : TopoIteratorBase() {
}

RevTopoIterator::RevTopoIterator(NodeBase* pNode)
  : TopoIteratorBase() {
  InitOrderList(pNode);
  setNode(m_OrderList[m_Idx]);
}

RevTopoIterator::RevTopoIterator(const NodeBase* pNode)
  : TopoIteratorBase() {
  InitOrderList(const_cast<NodeBase*>(pNode));
  setNode(m_OrderList[m_Idx]);
}

void RevTopoIterator::doPushAllChildren(NodeBase& pRoot, Visited& pV, Stack& pS)
{
  ArcBase* iter = pRoot.first_in;
  while (nullptr != iter) {
    // if not visited
    if (pV.end() == pV.find(iter->source)) {
      pS.push_back(std::make_pair(false, iter->source));
    }
    iter = iter->next_in;
  }
}
