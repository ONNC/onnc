//===- TopK.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/TopK.h>

using namespace onnc;

char TopK::ID = 0;

//===----------------------------------------------------------------------===//
// TopK
//===----------------------------------------------------------------------===//
TopK::TopK(const IntAttr& pK)
  : ComputeOperator("TopK", ID),
    m_Axis(-1),
    m_K(pK) {
}

TopK::TopK(const IntAttr& pAxis,
           const IntAttr& pK)
  : ComputeOperator("TopK", ID),
    m_Axis(pAxis),
    m_K(pK) {
}

TopK::TopK(const TopK& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_K(pCopy.getK()) {
}

void TopK::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getK() << ">";
}

bool TopK::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
