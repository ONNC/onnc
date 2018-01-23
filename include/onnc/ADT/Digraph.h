//===- Digraph.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_LIST_DIGRAPH_H
#define ONNC_ADT_LIST_DIGRAPH_H
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
class Digraph
{
public:
  struct Node;
  struct Arc;

  struct Node {
  public:
    Node();

  public:
    Node *prev, *next;
    Arc *first_in, *first_out;
  };

  struct Arc {
  public:
    Arc();

  public:
    Node *target, *source;
    Arc *prev_in, *next_in;
    Arc *prev_out, *next_out;
  };

public:
  Digraph();

  Node* addNode();

  Arc* addArc(Node& pU, Node& pV);

  Arc* connect(Node& pU, Node& pV) { return addArc(pU, pV); }

  void erase(Node& pNode);

  void erase(Arc& pArc);

  void clear();

  void getHead(Node*& pNode) const { pNode = m_pNodeHead; }

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

} // namespace onnc

#endif
