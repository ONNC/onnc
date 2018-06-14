//===- SpaceToDepth.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/SpaceToDepth.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SpaceToDepth
//===----------------------------------------------------------------------===//
SpaceToDepth::SpaceToDepth()
  : ComputeOperator("SpaceToDepth"),
    m_Blocksize() {
}

SpaceToDepth::SpaceToDepth(const IntAttr& pBlocksize)
  : ComputeOperator("SpaceToDepth"),
    m_Blocksize(pBlocksize) {
}

void SpaceToDepth::print(std::ostream& pOS) const
{
}
