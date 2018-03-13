//===- DigraphArc.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_ARC_H
#define ONNC_ADT_DIGRAPH_ARC_H
#include <vector>

namespace onnc {

class NodeBase;
class ArcBase;

/** \class DigraphArc
 *  \brief DigraphArc provides arcs for class Digraph.
 */
class ArcBase
{
public:
  ArcBase();

public:
  NodeBase* target;
  NodeBase* source;
  ArcBase* prev_in;
  ArcBase* next_in;
  ArcBase* prev_out;
  ArcBase* next_out;
};

template<typename NodeType, typename ArcType>
class DigraphArc : public ArcBase
{
public:
  DigraphArc() : ArcBase() { }

  const NodeType* getTarget() const;

  NodeType* getTarget();

  const NodeType* getSource() const;

  NodeType* getSource();

  const ArcType* getPrevIn() const;

  ArcType* getPrevIn();

  const ArcType* getNextIn() const;

  ArcType* getNextIn();

  const ArcType* getPrevOut() const;

  ArcType* getPrevOut();

  const ArcType* getNextOut() const;

  ArcType* getNextOut();
};

//===----------------------------------------------------------------------===//
// Template Implementation
//===----------------------------------------------------------------------===//
template<typename NodeType, typename ArcType>
const NodeType* DigraphArc<NodeType, ArcType>::getTarget() const
{
  return static_cast<const NodeType*>(target);
}

template<typename NodeType, typename ArcType>
NodeType* DigraphArc<NodeType, ArcType>::getTarget() {
  return static_cast<NodeType*>(target);
}

template<typename NodeType, typename ArcType>
const NodeType* DigraphArc<NodeType, ArcType>::getSource() const {
  return static_cast<const NodeType*>(source);
}

template<typename NodeType, typename ArcType>
NodeType* DigraphArc<NodeType, ArcType>::getSource() {
  return static_cast<NodeType*>(source);
}

template<typename NodeType, typename ArcType>
const ArcType* DigraphArc<NodeType, ArcType>::getPrevIn() const {
  return static_cast<const ArcType*>(prev_in);
}

template<typename NodeType, typename ArcType>
ArcType* DigraphArc<NodeType, ArcType>::getPrevIn() {
  return static_cast<ArcType*>(prev_in);
}

template<typename NodeType, typename ArcType>
  const ArcType* DigraphArc<NodeType, ArcType>::getNextIn() const {
    return static_cast<const ArcType*>(next_in);
  }

template<typename NodeType, typename ArcType>
  ArcType* DigraphArc<NodeType, ArcType>::getNextIn() { return static_cast<ArcType*>(next_in); }

template<typename NodeType, typename ArcType>
  const ArcType* DigraphArc<NodeType, ArcType>::getPrevOut() const {
    return static_cast<const ArcType*>(prev_out);
  }

template<typename NodeType, typename ArcType>
  ArcType* DigraphArc<NodeType, ArcType>::getPrevOut() { return static_cast<ArcType*>(prev_out); }

template<typename NodeType, typename ArcType>
  const ArcType* DigraphArc<NodeType, ArcType>::getNextOut() const {
    return static_cast<const ArcType*>(next_out);
  }

template<typename NodeType, typename ArcType>
ArcType* DigraphArc<NodeType, ArcType>::getNextOut() { return static_cast<ArcType*>(next_out); }

} // namespace onnc

#endif
