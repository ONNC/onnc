//===- GlobalLpPool.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalLpPool.h>

using namespace onnc;

char GlobalLpPool::ID = 0;

//===----------------------------------------------------------------------===//
// GlobalLpPool
//===----------------------------------------------------------------------===//
GlobalLpPool::GlobalLpPool()
  : ComputeOperator("GlobalLpPool", ID),
    m_P(2) {
}

GlobalLpPool::GlobalLpPool(const IntAttr& pP)
  : ComputeOperator("GlobalLpPool", ID),
    m_P(pP) {
}

GlobalLpPool::GlobalLpPool(const GlobalLpPool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_P(pCopy.getP()) {
}

void GlobalLpPool::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "p: " << getP()<< '>';
}

bool GlobalLpPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
