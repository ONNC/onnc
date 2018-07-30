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
MaxRoiPool::MaxRoiPool(const IntsAttr& pPooledShape)
  : ComputeOperator("MaxRoiPool", ID),
    m_PooledShape(pPooledShape),
    m_SpatialScale(1.0) {
}

MaxRoiPool::MaxRoiPool(const IntsAttr& pPooledShape,
                       const FloatAttr& pSpatialScale)
  : ComputeOperator("MaxRoiPool", ID),
    m_PooledShape(pPooledShape),
    m_SpatialScale(pSpatialScale) {
}

MaxRoiPool::MaxRoiPool(const MaxRoiPool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_PooledShape(pCopy.getPooledShape()),
    m_SpatialScale(pCopy.getSpatialScale()) {
}

void MaxRoiPool::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getPooledShape() << ", " << getSpatialScale() << ">";
}

bool MaxRoiPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
