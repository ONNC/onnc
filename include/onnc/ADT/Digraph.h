//===- Digraph.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_H
#define ONNC_ADT_DIGRAPH_H
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/ADT/Bits/DigraphArc.h>
#include <onnc/ADT/Bits/PolicyNodeIterator.h>
#include <onnc/ADT/NodeIterator.h>
#include <onnc/ADT/TypeTraits.h>
#include <vector>

namespace onnc {

/** \class Digraph
 *  \brief Digraph provides an linked-list inplementation of a graph.
 *
 *  Digraph is designed to get well performance for most algorithms of
 *  graph theory.
 *
 *  Function        | Complexity | Best Complexity
 *  ----------------|------------|--------------------------
 *  Storage         | V + E      |
 *  Add node        | O(1)       |
 *  Add arc         | O(1)       |
 *  Remove node     | O(E)       | O(#(fan-in) + #(fan-out))
 *  Remove edge     | O(1)       |
 *  Query adjacency | O(E)       | O(#(fan-in) + #(fan-out))
 *
 */
template<typename NodeType = digraph::NodeBase, typename ArcType = digraph::ArcBase>
class Digraph
{
public:
  typedef NodeType Node;
  typedef ArcType  Arc;
  typedef NodeIterator<NodeType> iterator;
  typedef NodeIterator<const NodeType> const_iterator;
  typedef digraph::PolicyNodeIterator<digraph::DFSIterator, NonConstTraits<NodeType> > dfs_iterator;
  typedef digraph::PolicyNodeIterator<digraph::DFSIterator, ConstTraits<NodeType> > const_dfs_iterator;
  typedef digraph::PolicyNodeIterator<digraph::BFSIterator, NonConstTraits<NodeType> > bfs_iterator;
  typedef digraph::PolicyNodeIterator<digraph::BFSIterator, ConstTraits<NodeType> > const_bfs_iterator;

public:
  Digraph();

  virtual ~Digraph();

  template<typename ... NodeCtorParams>
  Node* addNode(NodeCtorParams&& ... pParams);

  template<typename ... ArcCtorParams>
  Arc* addArc(Node& pU, Node& pV, ArcCtorParams&& ... pParams);

  template<typename ... ArcCtorParams>
  Arc* connect(Node& pU, Node& pV, ArcCtorParams&& ... pParams) {
    return addArc(pU, pV, pParams...);
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

  bool exists(const Node& pNode) const;

private:
  typedef std::vector<Node*> NodeList;
  typedef std::vector<Arc*> ArcList;

private:
  Node* m_pNodeHead;
  Node* m_pNodeRear;
  Node* m_pFreeNodeHead; //< list of free nodes
  Arc*  m_pFreeArcHead;  //< list of free arcs

  NodeList m_NodeList;
  ArcList m_ArcList;
};

#include "Bits/Digraph.tcc"

} // namespace onnc

#endif
