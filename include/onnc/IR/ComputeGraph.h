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
#include <onnc/ADT/StringMap.h>
#include <onnc/ADT/TypeTraits.h>
#include <onnc/Support/MemoryPool.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <onnc/Support/Casting.h>
#include <set>

namespace onnc {

class Module;

/** \class ComputeGraph
 */
class ComputeGraph
{
public:
  typedef std::unordered_set<ComputeOperand*> ArcList;

  typedef StringMap<Value*> ValueList;

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
  ComputeGraph(const std::string& pName, Module& pModule, ArcList& pArcList);

  virtual ~ComputeGraph();

  const std::string& name() const { return m_Name; }

  template<typename OpType, typename ... NodeCtorParams>
  OpType* addOperator(NodeCtorParams&& ... pParams);

  /// Add an operator in the graph. The added node's life cycle is delegated
  /// to the graph. ComputeGraph is responsible for destruction of the added
  /// node.
  template<typename OpType>
  ComputeGraph& addOperator(OpType& pOperator);

  template<typename ValueType, typename ... ValueCtorParams>
  ValueType* addValue(ValueCtorParams&& ... pParams);

  template<typename ValueType = onnc::Value>
  ValueType* getValue(StringRef pName);

  template<typename ValueType = onnc::Value>
  const ValueType* getValue(StringRef pName) const;

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

  unsigned int getArcSize() const { return m_ArcList.size(); }

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
  void addValueToModule(Value* pValue);

private:
  Module& m_Module;
  std::string m_Name;
  Node* m_pNodeHead;
  Node* m_pNodeRear;
  NodeList m_NodeList;
  ArcList& m_ArcList;
  ValueList& m_ValueList;
};

#include "Bits/ComputeGraph.tcc"

} // namespace of onnc

#endif
