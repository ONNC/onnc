//===- MaxRoiPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MaxRoiPool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MaxRoiPool
//===----------------------------------------------------------------------===//
MaxRoiPool::MaxRoiPool()
  : ComputeOperator("MaxRoiPool"),
    m_PooledShape(),
    m_SpatialScale() {
}

MaxRoiPool::MaxRoiPool(const IntsAttr& pPooledShape,
                       const FloatAttr& pSpatialScale)
  : ComputeOperator("MaxRoiPool"),
    m_PooledShape(pPooledShape),
    m_SpatialScale(pSpatialScale) {
}

void MaxRoiPool::print(std::ostream& pOS) const
{
}
