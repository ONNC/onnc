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
digraph::NodeBase::NodeBase()
  : prev(nullptr), next(nullptr),
    first_in(nullptr), last_in(nullptr),
    first_out(nullptr), last_out(nullptr) {
}

//===----------------------------------------------------------------------===//
// ArcBase
//===----------------------------------------------------------------------===//
digraph::ArcBase::ArcBase()
  : target(nullptr), source(nullptr),
    prev_in(nullptr), next_in(nullptr),
    prev_out(nullptr), next_out(nullptr) {
}
