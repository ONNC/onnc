//===- ComputeGraph.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_GRAPH_H
#define ONNC_IR_COMPUTE_GRAPH_H
#include <onnc/ADT/Bits/PolicyNodeIterator.h>
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Support/MemoryPool.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <set>

namespace onnc {

class Module;

/** \class ComputeGraph
 */
class ComputeGraph
{
public:
  typedef std::unordered_set<ComputeOperand*> ArcList;

public:
  typedef ComputeOperator Node;
  typedef ComputeOperand  Arc;

  typedef NodeIterator<Node, NonConstTraits> iterator;
  typedef NodeIterator<Node, ConstTraits> const_iterator;

  typedef digraph::PolicyNodeIterator<digraph::DFSIterator,
                                      NonConstTraits<Node> > dfs_iterator;

  typedef digraph::PolicyNodeIterator<digraph::DFSIterator,
                                      ConstTraits<Node> > const_dfs_iterator;

  typedef digraph::PolicyNodeIterator<digraph::BFSIterator,
                                      NonConstTraits<Node> > bfs_iterator;

  typedef digraph::PolicyNodeIterator<digraph::BFSIterator,
                                      ConstTraits<Node> > const_bfs_iterator;

public:
  ComputeGraph(Module& pModule, ArcList& pArcList);

  virtual ~ComputeGraph();

  template<typename OpType, typename ... NodeCtorParams>
  OpType* addOperator(NodeCtorParams&& ... pParams);

  template<typename OpndType, typename ... ArcCtorParams>
  OpndType* addOperand(Node& pU, Node& pV, ArcCtorParams&& ... pParams);

  template<typename OpndType, typename ... ArcCtorParams>
  OpndType* connect(Node& pU, Node& pV, ArcCtorParams&& ... pParams) {
    return addOperand<OpndType>(pU, pV, pParams...);
  }

  void erase(Node& pNode);

  void erase(Arc& pArc);

  void clear();

  void getRear(Node*& pNode) const { pNode = m_pNodeRear; }

  void getHead(Node*& pNode) const { pNode = m_pNodeHead; }

  const Node* head() const { return m_pNodeHead; }

  Node* head() { return m_pNodeHead; }

  bool hasHead() const { return (nullptr != m_pNodeHead); }

  unsigned int getNodeSize() const { return m_NodeList.size(); }

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  dfs_iterator dfs_begin();

  dfs_iterator dfs_end();

  const_dfs_iterator dfs_begin() const;

  const_dfs_iterator dfs_end() const;

  bfs_iterator bfs_begin();

  bfs_iterator bfs_end();

  const_bfs_iterator bfs_begin() const;

  const_bfs_iterator bfs_end() const;

private:
  typedef std::unordered_set<Node*> NodeList;

private:
  Module& m_Module;
  Node* m_pNodeHead;
  Node* m_pNodeRear;
  NodeList m_NodeList;
  ArcList& m_ArcList;
};

#include "Bits/ComputeGraph.tcc"

} // namespace of onnc

#endif
