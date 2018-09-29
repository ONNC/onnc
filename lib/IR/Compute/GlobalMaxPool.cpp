//===- GlobalMaxPool.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalMaxPool.h>

using namespace onnc;

char GlobalMaxPool::ID = 0;

//===----------------------------------------------------------------------===//
// GlobalMaxPool
//===----------------------------------------------------------------------===//
GlobalMaxPool::GlobalMaxPool()
  : ComputeOperator("GlobalMaxPool", ID) {
}



GlobalMaxPool::GlobalMaxPool(const GlobalMaxPool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void GlobalMaxPool::printAttributes(std::ostream& pOS) const
{
  ;
}

bool GlobalMaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
