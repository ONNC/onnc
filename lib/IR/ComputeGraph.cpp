//===- ComputeGraph.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeGraph.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeGraph
//===----------------------------------------------------------------------===//
ComputeGraph::ComputeGraph()
{
}

ComputeGraph::~ComputeGraph()
{
  // factories delete elements in their destructors.
}
