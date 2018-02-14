//===- Digraph.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_H
#define ONNC_ADT_DIGRAPH_H
#include <onnc/ADT/DigraphNode.h>
#include <onnc/ADT/DigraphArc.h>
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
template<typename NodeType, typename ArcType>
class Digraph
{
public:
  typedef NodeType Node;
  typedef ArcType  Arc;

public:
  Digraph();

  Node* addNode();

  Arc* addArc(Node& pU, Node& pV);

  Arc* connect(Node& pU, Node& pV) { return addArc(pU, pV); }

  void erase(Node& pNode);

  void erase(Arc& pArc);

  void clear();

  void getHead(Node*& pNode) const { pNode = m_pNodeHead; }

  unsigned int getNodeSize() const { return m_NodeList.size(); }

  unsigned int getArcSize() const { return m_ArcList.size(); }

private:
  typedef std::vector<Node> NodeList;
  typedef std::vector<Arc> ArcList;

private:
  Node* m_pNodeHead;
  Node* m_pFreeNodeHead; //< list of free nodes
  Arc*  m_pFreeArcHead;  //< list of free arcs

  NodeList m_NodeList;
  ArcList m_ArcList;
};

#include "Bits/Digraph.tcc"

} // namespace onnc

#endif
