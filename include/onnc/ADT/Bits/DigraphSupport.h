//===- DigraphSupport.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_DIGRAPH_SUPPORT_H
#define ONNC_ADT_DIGRAPH_SUPPORT_H
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/ADT/Bits/DigraphArc.h>

namespace onnc {

template<typename NodeType, typename ArcType>
unsigned int DigraphNode<NodeType, ArcType>::getNumOfOutArcs() const
{
  unsigned int res = 0;
  digraph::ArcBase* arc = last_out;
  while (nullptr != arc) {
    ++res;
    arc = arc->prev_out;
  }
  return res;
}

template<typename NodeType, typename ArcType>
unsigned int DigraphNode<NodeType, ArcType>::getNumOfInArcs() const
{
  unsigned int res = 0;
  digraph::ArcBase* arc = last_in;
  while (nullptr != arc) {
    ++res;
    arc = arc->prev_in;
  }
  return res;
}

} // namespace of onnc

#endif
