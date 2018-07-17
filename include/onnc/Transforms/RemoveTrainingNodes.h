//===- removeUnusedNode.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef REMOVE_UNUSED_NODES
#define REMOVE_UNUSED_NODES

#include <onnc/Core/ModulePass.h>

namespace onnc {
ModulePass *createRemoveUnusedNodesPass();
}

#endif // REMOVE_UNUSED_NODES
