#ifndef ONNC_IR_COMPUTE_GRAPH_SUPPORT_H
#define ONNC_IR_COMPUTE_GRAPH_SUPPORT_H
//===-- ComputeGraphSupport.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnc/IR/ComputeGraph.h>

namespace onnc {

template<typename OpType, class ... NodeCtorParams>
OpType* ComputeGraph::addOperator(NodeCtorParams&& ... pParams)
{
  // 1. find an available free node
  OpType* result = nullptr;
  if (nullptr == m_pFreeNodeHead) {
    result = new OpType(pParams...);
    m_NodeList.push_back(result);
  }
  else {
    result = m_pFreeNodeHead;
    m_pFreeNodeHead = static_cast<Node*>(m_pFreeNodeHead->next);
  }

  // 2. set up linkages
  result->prev = m_pNodeRear;
  result->next = nullptr;
  result->first_in = nullptr;
  result->last_in = nullptr;
  result->first_out = nullptr;
  result->last_out = nullptr;

  // 3. reset rear node
  if (nullptr != m_pNodeRear) {
    m_pNodeRear->next = result;
  }
  m_pNodeRear = result;

  if (nullptr == m_pNodeHead)
    m_pNodeHead = result;

  return result;
}

template<typename OpndType, typename ... ArcCtorParams>
OpndType*
ComputeGraph::addOperand(Node& pU, Node& pV, ArcCtorParams&& ... pParams)
{
  // 1. find an available free arc
  OpndType* result = new OpndType(pParams...);
  m_Module.getComputeOperands().push_back(result);

  // 2. set up arc
  result->source = &pU;
  result->target = &pV;
  result->prev_in = nullptr;
  result->next_in = nullptr;
  result->prev_out = nullptr;
  result->next_out = nullptr;

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

} // namespace of onnc

#endif
