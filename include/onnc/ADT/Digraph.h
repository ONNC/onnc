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
#include <onnc/ADT/NodeIterator.h>
#include <onnc/ADT/PolicyNodeIterator.h>
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
template<typename NodeType, typename ArcType>
class Digraph
{
public:
  typedef NodeType Node;
  typedef ArcType  Arc;
  typedef NodeIterator<NodeType> iterator;
  typedef NodeIterator<const NodeType> const_iterator;
  typedef PolicyNodeIterator<DSFIterator, NonConstTraits<NodeType> > dsf_iterator;
  typedef PolicyNodeIterator<DSFIterator, ConstTraits<NodeType> > const_dsf_iterator;
  typedef PolicyNodeIterator<BSFIterator, NonConstTraits<NodeType> > bsf_iterator;
  typedef PolicyNodeIterator<BSFIterator, ConstTraits<NodeType> > const_bsf_iterator;

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

  unsigned int getNodeSize() const { return m_NodeList.size(); }

  unsigned int getArcSize() const { return m_ArcList.size(); }

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  dsf_iterator dsf_begin();

  dsf_iterator dsf_end();

  const_dsf_iterator dsf_begin() const;

  const_dsf_iterator dsf_end() const;

  bsf_iterator bsf_begin();

  bsf_iterator bsf_end();

  const_bsf_iterator bsf_begin() const;

  const_bsf_iterator bsf_end() const;

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
