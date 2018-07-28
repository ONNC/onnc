//===- MaxRoiPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MaxRoiPool.h>

using namespace onnc;

char MaxRoiPool::ID = 0;

//===----------------------------------------------------------------------===//
// MaxRoiPool
//===----------------------------------------------------------------------===//
MaxRoiPool::MaxRoiPool()
  : ComputeOperator("MaxRoiPool", ID),
    m_PooledShape(),
    m_SpatialScale() {
}

MaxRoiPool::MaxRoiPool(const IntsAttr& pPooledShape,
                       const FloatAttr& pSpatialScale)
  : ComputeOperator("MaxRoiPool", ID),
    m_PooledShape(pPooledShape),
    m_SpatialScale(pSpatialScale) {
}

void MaxRoiPool::print(std::ostream& pOS) const
{
}

bool MaxRoiPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
