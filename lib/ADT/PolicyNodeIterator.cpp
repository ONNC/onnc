//===-- PolicyNodeIterator.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Bits/PolicyNodeIterator.h>
#include <onnc/ADT/ArcIterator.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;
using namespace onnc::digraph;

//===----------------------------------------------------------------------===//
// BFSIterator
//===----------------------------------------------------------------------===//
BFSIterator::BFSIterator()
  : NodeIteratorBase() {
}

BFSIterator::BFSIterator(NodeBase* pNode)
  : NodeIteratorBase(pNode) {
  m_Queue.push(pNode);
  m_Visited.insert(pNode);
}

bool BFSIterator::isEnd() const
{
  return m_Queue.empty();
}

void BFSIterator::advance()
{
  // get and remove node from top
  NodeBase* node = m_Queue.front();
  m_Queue.pop();

  // Get all adjacent nodes of the dequeued node.
  // If a adjancent node has not been visited, then mark it visited and
  // enqueue it.
  ArcBase* arc = node->first_out;
  while (nullptr != arc) {
    // insert only if the node isn't visited and all fan-in arcs are fresh
    bool shall_insert = (m_Visited.end() == m_Visited.find(arc->target));
    ArcBase* in = arc->target->first_in;
    while (nullptr != in) {
      if (m_Visited.end() == m_Visited.find(in->source)) {
        shall_insert = false;
        break;
      }
      in = in->next_in;
    }

    if (shall_insert)
      m_Queue.push(arc->target);
    arc = arc->next_out;
  }

  if (m_Queue.empty()) { // reach the end
    setNode(nullptr);
    m_Visited.clear();
  }
  else {
    m_Visited.insert(m_Queue.front());
    setNode(m_Queue.front());
  }
}

//===----------------------------------------------------------------------===//
// DFSIterator
//===----------------------------------------------------------------------===//
DFSIterator::DFSIterator()
  : NodeIteratorBase() {
}

DFSIterator::DFSIterator(NodeBase* pNode)
  : NodeIteratorBase(pNode) {
  m_Stack.push(pNode);
  m_Visited.insert(pNode);
}

bool DFSIterator::isEnd() const
{
  return m_Stack.empty();
}

void DFSIterator::advance()
{
  // get and remove node from top
  NodeBase* node = m_Stack.top();
  m_Stack.pop();

  // Get all adjacent nodes of the destacked node.
  // If a adjancent node has not been visited, then mark it visited and
  // push it into stack.
  ArcBase* arc = node->last_out;
  while (nullptr != arc) {
    // insert only if the node isn't visited and all fan-in arcs are fresh
    bool shall_insert = (m_Visited.end() == m_Visited.find(arc->target));
    ArcBase* in = arc->target->first_in;
    while (nullptr != in) {
      if (m_Visited.end() == m_Visited.find(in->source)) {
        shall_insert = false;
        break;
      }
      in = in->next_in;
    }
    if (shall_insert)
      m_Stack.push(arc->target);
    arc = arc->prev_out;
  }

  if (m_Stack.empty()) { // reach the end of iterator
    setNode(nullptr);
    m_Visited.clear();
  }
  else {
    m_Visited.insert(m_Stack.top());
    setNode(m_Stack.top());
  }
}
