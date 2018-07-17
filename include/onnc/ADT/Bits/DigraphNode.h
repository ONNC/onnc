//===- DigraphNode.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_NODE_H
#define ONNC_ADT_DIGRAPH_NODE_H

namespace onnc {
namespace digraph {

struct NodeBase;
class ArcBase;

/** \class DigraphNode
 *  \brief DigraphNode provides nodes for class Digraph.
 */
struct NodeBase
{
public:
  NodeBase();

public:
  NodeBase *prev, *next;
  ArcBase *first_in, *last_in;
  ArcBase *first_out, *last_out;
};

} // namespace of digraph

template<typename NodeType, typename ArcType = digraph::ArcBase>
class DigraphNode : public digraph::NodeBase
{
public:
  DigraphNode() : NodeBase() { }

  const NodeType* getPrevNode() const;

  NodeType* getPrevNode();

  const NodeType* getNextNode() const;

  NodeType* getNextNode();

  const ArcType* getFirstInArc() const;

  ArcType* getFirstInArc();

  const ArcType* getFirstOutArc() const;

  ArcType* getFirstOutArc();

  const ArcType* getLastInArc() const;

  ArcType* getLastInArc();

  const ArcType* getLastOutArc() const;

  ArcType* getLastOutArc();

  /// include <onnc/ADT/Bits/DigraphSupport.h>
  /// time complexity O(n)
  unsigned int getNumOfOutArcs() const;

  /// include <onnc/ADT/Bits/DigraphSupport.h>
  /// time complexity O(n)
  unsigned int getNumOfInArcs() const;
};

//===----------------------------------------------------------------------===//
// Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType, typename ArcType>
const NodeType* DigraphNode<NodeType, ArcType>::getPrevNode() const
{
  return static_cast<const NodeType*>(prev);
}

template<typename NodeType, typename ArcType>
NodeType* DigraphNode<NodeType, ArcType>::getPrevNode()
{
  return static_cast<NodeType*>(prev);
}

template<typename NodeType, typename ArcType>
const NodeType* DigraphNode<NodeType, ArcType>::getNextNode() const
{
  return static_cast<const NodeType*>(next);
}

template<typename NodeType, typename ArcType>
NodeType* DigraphNode<NodeType, ArcType>::getNextNode()
{
  return static_cast<NodeType*>(next);
}

template<typename NodeType, typename ArcType>
const ArcType* DigraphNode<NodeType, ArcType>::getFirstInArc() const
{
  return static_cast<const ArcType*>(first_in);
}

template<typename NodeType, typename ArcType>
ArcType* DigraphNode<NodeType, ArcType>::getFirstInArc()
{
  return static_cast<ArcType*>(first_in);
}

template<typename NodeType, typename ArcType>
const ArcType* DigraphNode<NodeType, ArcType>::getFirstOutArc() const
{
  return static_cast<const ArcType*>(first_out);
}

template<typename NodeType, typename ArcType>
ArcType* DigraphNode<NodeType, ArcType>::getFirstOutArc()
{
  return static_cast<ArcType*>(first_out);
}

template<typename NodeType, typename ArcType>
const ArcType* DigraphNode<NodeType, ArcType>::getLastInArc() const
{
  return static_cast<const ArcType*>(last_in);
}

template<typename NodeType, typename ArcType>
ArcType* DigraphNode<NodeType, ArcType>::getLastInArc()
{
  return static_cast<ArcType*>(last_in);
}

template<typename NodeType, typename ArcType>
const ArcType* DigraphNode<NodeType, ArcType>::getLastOutArc() const
{
  return static_cast<const ArcType*>(last_out);
}

template<typename NodeType, typename ArcType>
ArcType* DigraphNode<NodeType, ArcType>::getLastOutArc()
{
  return static_cast<ArcType*>(last_out);
}

} // namespace of onnc

#endif
