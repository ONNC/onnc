//===- SpaceToDepth.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/SpaceToDepth.h>

using namespace onnc;

char SpaceToDepth::ID = 0;

//===----------------------------------------------------------------------===//
// SpaceToDepth
//===----------------------------------------------------------------------===//
SpaceToDepth::SpaceToDepth(const IntAttr& pBlocksize)
  : ComputeOperator("SpaceToDepth", ID),
    m_Blocksize(pBlocksize) {
}



SpaceToDepth::SpaceToDepth(const SpaceToDepth& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Blocksize(pCopy.getBlocksize()) {
}

void SpaceToDepth::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "blocksize: " << getBlocksize()<< '>';
}

bool SpaceToDepth::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
