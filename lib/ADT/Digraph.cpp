//===-- Digraph.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/Digraph.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// NodeBase
//===----------------------------------------------------------------------===//
NodeBase::NodeBase()
  : prev(nullptr), next(nullptr), first_in(nullptr), first_out(nullptr) {
}

//===----------------------------------------------------------------------===//
// ArcBase
//===----------------------------------------------------------------------===//
ArcBase::ArcBase()
  : target(nullptr), source(nullptr),
    prev_in(nullptr), next_in(nullptr),
    prev_out(nullptr), next_out(nullptr) {
}
