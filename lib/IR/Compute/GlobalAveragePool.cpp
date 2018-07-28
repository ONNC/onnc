//===- GlobalAveragePool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalAveragePool.h>

using namespace onnc;

char GlobalAveragePool::ID = 0;

//===----------------------------------------------------------------------===//
// GlobalAveragePool
//===----------------------------------------------------------------------===//
GlobalAveragePool::GlobalAveragePool()
  : ComputeOperator("GlobalAveragePool", ID) {
}



void GlobalAveragePool::print(std::ostream& pOS) const
{
}

bool GlobalAveragePool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
