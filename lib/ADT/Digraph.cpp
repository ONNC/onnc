//===-- Digraph.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Digraph.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Digraph::Node
//===----------------------------------------------------------------------===//
Digraph::Node::Node()
  : prev(nullptr), next(nullptr), first_in(nullptr), first_out(nullptr) {
}

//===----------------------------------------------------------------------===//
// Digraph::Arc
//===----------------------------------------------------------------------===//
Digraph::Arc::Arc()
  : target(nullptr), source(nullptr),
    prev_in(nullptr), next_in(nullptr), prev_out(nullptr), next_out(nullptr) {
}

//===----------------------------------------------------------------------===//
// Digraph
//===----------------------------------------------------------------------===//
Digraph::Digraph()
  : m_pNodeHead(nullptr), m_pFreeNodeHead(nullptr), m_pFreeArcHead(nullptr),
    m_NodeList(), m_ArcList() {
}

Digraph::Node* Digraph::addNode()
{
  // 1. find an available free node
  Node* result = nullptr;
  if (nullptr == m_pFreeNodeHead) {
    m_NodeList.emplace_back();
    result = &m_NodeList.back();
  }
  else {
    result = m_pFreeNodeHead;
    m_pFreeNodeHead = m_pFreeNodeHead->next;
  }

  // 2. set up linkages
  result->prev = nullptr;
  result->next = m_pNodeHead;

  // 3. reset head node
  if (nullptr != m_pNodeHead) {
    m_pNodeHead->prev = result;
  }
  m_pNodeHead = result;

  return result;
}

Digraph::Arc* Digraph::addArc(Node& pU, Node& pV)
{
  // 1. find an available free arc
  Arc* result = nullptr;
  if (nullptr == m_pFreeArcHead) {
    m_ArcList.emplace_back();
    result = &m_ArcList.back();
  }
  else {
    result = m_pFreeArcHead;
    m_pFreeArcHead = m_pFreeArcHead->next_in;
  }

  // 2. set up arc
  result->source = &pU;
  result->target = &pV;

  // 3. set up fan-out linked list
  result->next_out = pU.first_out;
  if (nullptr != pU.first_out) {
    pU.first_out->prev_out = result;
  }
  pU.first_out = result;

  // 4. set up fan-in linked list
  result->next_in = pV.first_in;
  if (nullptr != pV.first_in) {
    pV.first_in->prev_in = result;
  }
  pV.first_in = result;

  return result;
}

void Digraph::erase(Digraph::Node& pNode)
{
  // 1. connect previous node and next node.
  if (nullptr != pNode.next) {
    pNode.next->prev = pNode.prev;
  }

  if (nullptr != pNode.prev) {
    pNode.prev->next = pNode.next;
  }
  else { // pNode.prev is NULL => pNode is the head
    m_pNodeHead = pNode.next;
  }

  // 2. remove all fan-in arcs
  Arc* fan_in = pNode.first_in;
  while(nullptr != fan_in) {
    Arc* next_in = fan_in->next_in;
    erase(*fan_in);
    fan_in = next_in;
  }

  // 3. remove all fan-out arcs
  Arc* fan_out = pNode.first_out;
  while(nullptr != fan_out) {
    Arc* next_out = fan_out->next_out;
    erase(*fan_out);
    fan_out = next_out;
  }

  // 4. put pNode in the free node list
  pNode.next = m_pFreeNodeHead;
  pNode.prev = nullptr;
  if (nullptr != m_pFreeNodeHead)
    m_pFreeNodeHead->prev = &pNode;
  m_pFreeNodeHead = &pNode;
}

void Digraph::erase(Digraph::Arc& pArc)
{
  // 1. remove from the fan-out list
  if (nullptr != pArc.prev_out) {
    pArc.prev_out->next_out = pArc.next_out;
  }
  else { // pArc.prev_out is NULL => pArc is the first_out of the source
    pArc.source->first_out = pArc.next_out;
  }

  if (nullptr != pArc.next_out) {
    pArc.next_out->prev_out = pArc.prev_out;
  }

  // 2. remove from the fan-in list
  if (nullptr != pArc.prev_in) {
    pArc.prev_in->next_in = pArc.next_in;
  }
  else {
    pArc.target->first_in = pArc.next_in;
  }

  if (nullptr != pArc.next_in) {
    pArc.next_in->prev_in = pArc.prev_in;
  }

  // 3. put pArc in the free arc list
  // Use fan-in links to chain the free list
  pArc.next_in = m_pFreeArcHead;
  m_pFreeArcHead = &pArc;
}

void Digraph::clear()
{
  m_pNodeHead = nullptr;
  m_pFreeNodeHead = nullptr;
  m_pFreeArcHead = nullptr;
  m_NodeList.clear();
  m_ArcList.clear();
}
