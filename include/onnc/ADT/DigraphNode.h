//===- DigraphNode.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_NODE_H
#define ONNC_ADT_DIGRAPH_NODE_H
#include <vector>

namespace onnc {

class NodeBase;
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
  ArcBase *first_in, *first_out;
};

template<typename NodeType, typename ArcType>
class DigraphNode : public NodeBase
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

} // namespace of onnc

#endif
