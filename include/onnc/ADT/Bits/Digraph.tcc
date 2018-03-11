//===-- Digraph.tcc -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Digraph
//===----------------------------------------------------------------------===//
template<typename NodeType, typename ArcType>
Digraph<NodeType, ArcType>::Digraph()
  : m_pNodeHead(nullptr), m_pNodeRear(nullptr),
    m_pFreeNodeHead(nullptr), m_pFreeArcHead(nullptr),
    m_NodeList(), m_ArcList() {
}

template<typename NodeType, typename ArcType>
Digraph<NodeType, ArcType>::~Digraph()
{
  clear();
}

template<typename NodeType, typename ArcType>
template<class ... NodeCtorParams>
typename Digraph<NodeType, ArcType>::Node*
Digraph<NodeType, ArcType>::addNode(NodeCtorParams&& ... pParams)
{
  // 1. find an available free node
  Node* result = nullptr;
  if (nullptr == m_pFreeNodeHead) {
    result = new NodeType(pParams...);
    m_NodeList.push_back(result);
  }
  else {
    result = m_pFreeNodeHead;
    m_pFreeNodeHead = m_pFreeNodeHead->getNextNode();
  }

  // 2. set up linkages
  result->prev = m_pNodeRear;
  result->next = nullptr;

  // 3. reset rear node
  if (nullptr != m_pNodeRear) {
    m_pNodeRear->next = result;
  }
  m_pNodeRear = result;

  if (nullptr == m_pNodeHead)
    m_pNodeHead = result;

  return result;
}

template<typename NodeType, typename ArcType>
template<typename ... ArcCtorParams>
typename Digraph<NodeType, ArcType>::Arc*
Digraph<NodeType, ArcType>::addArc(Node& pU, Node& pV, ArcCtorParams&& ... pParams)
{
  // 1. find an available free arc
  Arc* result = nullptr;
  if (nullptr == m_pFreeArcHead) {
    result = new ArcType(pParams...);
    m_ArcList.push_back(result);
  }
  else {
    result = m_pFreeArcHead;
    m_pFreeArcHead = m_pFreeArcHead->getNextIn();
  }

  // 2. set up arc
  result->source = &pU;
  result->target = &pV;

  // 3. set up fan-out linked list
  result->prev_out = pU.last_out;
  if (nullptr != pU.last_out) {
    pU.last_out->next_out = result;
  }
  else { // last_out is nullptr => a node without fan-out arcs.
    pU.first_out = result;
  }
  pU.last_out = result;

  // 4. set up fan-in linked list
  result->prev_in = pV.last_in;
  if (nullptr != pV.last_in) {
    pV.last_in->next_in = result;
  }
  else { // last_in is nullptr => a node without fan-in arcs
    pV.first_in = result;
  }
  pV.last_in = result;

  return result;
}

template<typename NodeType, typename ArcType>
void Digraph<NodeType, ArcType>::erase(Node& pNode)
{
  // 1. connect previous node and next node.
  if (nullptr != pNode.next) {
    pNode.next->prev = pNode.prev;
  }
  else { // pNode.next is NULL => pNode is the rear
    m_pNodeRear = pNode.getPrevNode();
  }

  if (nullptr != pNode.prev) {
    pNode.prev->next = pNode.next;
  }
  else { // pNode.prev is NULL => pNode is the head
    m_pNodeHead = pNode.getNextNode();
  }

  // 2. remove all fan-in arcs
  Arc* fan_in = pNode.getFirstInArc();
  while(nullptr != fan_in) {
    Arc* next_in = fan_in->getNextIn();
    erase(*fan_in);
    fan_in = next_in;
  }

  // 3. remove all fan-out arcs
  Arc* fan_out = pNode.getFirstOutArc();
  while(nullptr != fan_out) {
    Arc* next_out = fan_out->getNextOut();
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

template<typename NodeType, typename ArcType>
void Digraph<NodeType, ArcType>::erase(Arc& pArc)
{
  // 1. remove from the fan-out list
  if (nullptr != pArc.prev_out) {
    pArc.prev_out->next_out = pArc.next_out;
  }
  else { // pArc.prev_out is NULL => pArc is the first_out of the source
    pArc.source->first_out = pArc.next_out;
  }

  if (nullptr != pArc.next_out) { // a middle arc
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

template<typename NodeType, typename ArcType>
void Digraph<NodeType, ArcType>::clear()
{
  m_pNodeHead = nullptr;
  m_pNodeRear = nullptr;
  m_pFreeNodeHead = nullptr;
  m_pFreeArcHead = nullptr;

  // delete all nodes
  typename NodeList::iterator node, nEnd = m_NodeList.end();
  for (node = m_NodeList.begin(); node != nEnd; ++node)
    delete *node;

  // delete all arcs
  typename ArcList::iterator arc, aEnd = m_ArcList.end();
  for (arc = m_ArcList.begin(); arc != aEnd; ++arc)
    delete *arc;

  m_NodeList.clear();
  m_ArcList.clear();
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::iterator
Digraph<NodeType, ArcType>::begin()
{
  return iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::iterator
Digraph<NodeType, ArcType>::end()
{
  return iterator(nullptr);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_iterator
Digraph<NodeType, ArcType>::begin() const
{
  return const_iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_iterator
Digraph<NodeType, ArcType>::end() const
{
  return const_iterator(nullptr);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::dsf_iterator
Digraph<NodeType, ArcType>::dsf_begin()
{
  return dsf_iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::dsf_iterator
Digraph<NodeType, ArcType>::dsf_end()
{
  return dsf_iterator();
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_dsf_iterator
Digraph<NodeType, ArcType>::dsf_begin() const
{
  return const_dsf_iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_dsf_iterator
Digraph<NodeType, ArcType>::dsf_end() const
{
  return const_dsf_iterator();
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::bsf_iterator
Digraph<NodeType, ArcType>::bsf_begin()
{
  return bsf_iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::bsf_iterator
Digraph<NodeType, ArcType>::bsf_end()
{
  return bsf_iterator();
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_bsf_iterator
Digraph<NodeType, ArcType>::bsf_begin() const
{
  return const_bsf_iterator(m_pNodeHead);
}

template<typename NodeType, typename ArcType>
typename Digraph<NodeType, ArcType>::const_bsf_iterator
Digraph<NodeType, ArcType>::bsf_end() const
{
  return const_bsf_iterator();
}
