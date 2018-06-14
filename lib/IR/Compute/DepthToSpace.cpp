//===- DepthToSpace.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/DepthToSpace.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// DepthToSpace
//===----------------------------------------------------------------------===//
DepthToSpace::DepthToSpace()
  : ComputeOperator("DepthToSpace"),
    m_Blocksize() {
}

DepthToSpace::DepthToSpace(const IntAttr& pBlocksize)
  : ComputeOperator("DepthToSpace"),
    m_Blocksize(pBlocksize) {
}

void DepthToSpace::print(std::ostream& pOS) const
{
}
