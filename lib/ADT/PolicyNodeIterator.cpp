//===-- PolicyNodeIterator.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/PolicyNodeIterator.h>
#include <onnc/ADT/ArcIterator.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BSFIterator
//===----------------------------------------------------------------------===//
BSFIterator::BSFIterator()
  : NodeIteratorBase() {
}

BSFIterator::BSFIterator(NodeBase* pNode)
  : NodeIteratorBase(pNode) {
  m_Queue.push(pNode);
  m_Visited.insert(pNode);
}

bool BSFIterator::isEnd() const
{
  return m_Queue.empty();
}

void BSFIterator::advance()
{
  // get and remove node from top
  NodeBase* node = m_Queue.front();
  m_Queue.pop();

  // Get all adjacent nodes of the dequeued node.
  // If a adjancent node has not been visited, then mark it visited and
  // enqueue it.
  ArcBase* arc = node->first_out;
  while (nullptr != arc) {
    if (m_Visited.end() == m_Visited.find(arc->target)) { // not been visited
      m_Visited.insert(arc->target);
      m_Queue.push(arc->target);
    }
    arc = arc->next_out;
  }
  if (m_Queue.empty())
    setNode(nullptr);
  else
    setNode(m_Queue.front());
}

//===----------------------------------------------------------------------===//
// DSFIterator
//===----------------------------------------------------------------------===//
DSFIterator::DSFIterator()
  : NodeIteratorBase() {
}

DSFIterator::DSFIterator(NodeBase* pNode)
  : NodeIteratorBase(pNode) {
  m_Stack.push(pNode);
  m_Visited.insert(pNode);
}

bool DSFIterator::isEnd() const
{
  return m_Stack.empty();
}

void DSFIterator::advance()
{
  // get and remove node from top
  NodeBase* node = m_Stack.top();
  m_Stack.pop();

  // Get all adjacent nodes of the destacked node.
  // If a adjancent node has not been visited, then mark it visited and
  // push it into stack.
  ArcBase* arc = node->first_out;
  while (nullptr != arc) {
    if (m_Visited.end() == m_Visited.find(arc->target)) { // not been visited
      m_Visited.insert(arc->target);
      m_Stack.push(arc->target);
    }
    arc = arc->next_out;
  }

  if (m_Stack.empty())
    setNode(nullptr);
  else
    setNode(m_Stack.top());
}
