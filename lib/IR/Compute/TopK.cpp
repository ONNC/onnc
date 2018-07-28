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
TopK::TopK()
  : ComputeOperator("TopK", ID),
    m_Axis(),
    m_K() {
}

TopK::TopK(const IntAttr& pAxis,
           const IntAttr& pK)
  : ComputeOperator("TopK", ID),
    m_Axis(pAxis),
    m_K(pK) {
}

void TopK::print(std::ostream& pOS) const
{
}

bool TopK::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
