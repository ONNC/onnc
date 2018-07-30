//===- DepthToSpace.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/DepthToSpace.h>

using namespace onnc;

char DepthToSpace::ID = 0;

//===----------------------------------------------------------------------===//
// DepthToSpace
//===----------------------------------------------------------------------===//
DepthToSpace::DepthToSpace(const IntAttr& pBlocksize)
  : ComputeOperator("DepthToSpace", ID),
    m_Blocksize(pBlocksize) {
}



DepthToSpace::DepthToSpace(const DepthToSpace& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Blocksize(pCopy.getBlocksize()) {
}

void DepthToSpace::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getBlocksize() << ">";
}

bool DepthToSpace::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
