//===- ComputeGraph.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeGraph.h>
#include <onnc/IR/Module.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeGraph
//===----------------------------------------------------------------------===//
ComputeGraph::ComputeGraph(const std::string& pName,
                           Module& pModule,
                           ArcList& pArcList)
  : m_Module(pModule),
    m_Name(pName),
    m_pNodeHead(nullptr),
    m_pNodeRear(nullptr),
    m_NodeList(),
    m_ArcList(pArcList),
    m_ValueList(pModule.getValueList()) {
}

ComputeGraph::~ComputeGraph()
{
  clear();
}

void ComputeGraph::addValueToModule(Value* pValue)
{
  m_Module.addValue(pValue);
}

void ComputeGraph::erase(ComputeOperator& pNode)
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

  // 4. remove pNode from the node list
  m_NodeList.erase(&pNode);

  // 5. remove the memory space since it's delegated.
  delete &pNode;
}

void ComputeGraph::erase(ComputeOperand& pArc)
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

  // 3. remove from the arc list
  m_ArcList.erase(&pArc);

  // 4. remove the memory space since it is delegated.
  delete &pArc;
}

void ComputeGraph::clear()
{
  m_pNodeHead = nullptr;
  m_pNodeRear = nullptr;

  // delete nodes only because operands are deleted in module.
  NodeList::iterator node, nEnd = m_NodeList.end();
  for (node = m_NodeList.begin(); node != nEnd; ++node)
    delete *node;

  m_NodeList.clear();

  ArcList::iterator arc, aEnd = m_ArcList.end();
  for (arc = m_ArcList.begin(); arc != aEnd; ++arc)
    delete *arc;
  m_ArcList.clear();
}

ComputeGraph::iterator ComputeGraph::begin()
{
  return iterator(m_pNodeHead);
}

ComputeGraph::iterator ComputeGraph::end()
{
  return iterator(nullptr);
}

ComputeGraph::const_iterator ComputeGraph::begin() const
{
  return const_iterator(m_pNodeHead);
}

ComputeGraph::const_iterator ComputeGraph::end() const
{
  return const_iterator(nullptr);
}

ComputeGraph::dfs_iterator ComputeGraph::dfs_begin()
{
  return dfs_iterator(m_pNodeHead);
}

ComputeGraph::dfs_iterator ComputeGraph::dfs_end()
{
  return dfs_iterator();
}

ComputeGraph::const_dfs_iterator ComputeGraph::dfs_begin() const
{
  return const_dfs_iterator(m_pNodeHead);
}

ComputeGraph::const_dfs_iterator ComputeGraph::dfs_end() const
{
  return const_dfs_iterator();
}

ComputeGraph::bfs_iterator ComputeGraph::bfs_begin()
{
  return bfs_iterator(m_pNodeHead);
}

ComputeGraph::bfs_iterator ComputeGraph::bfs_end()
{
  return bfs_iterator();
}

ComputeGraph::const_bfs_iterator ComputeGraph::bfs_begin() const
{
  return const_bfs_iterator(m_pNodeHead);
}

ComputeGraph::const_bfs_iterator ComputeGraph::bfs_end() const
{
  return const_bfs_iterator();
}
