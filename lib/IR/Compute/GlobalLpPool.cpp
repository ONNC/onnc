//===- GlobalLpPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalLpPool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GlobalLpPool
//===----------------------------------------------------------------------===//
GlobalLpPool::GlobalLpPool()
  : ComputeOperator("GlobalLpPool"),
    m_P() {
}

GlobalLpPool::GlobalLpPool(const IntAttr& pP)
  : ComputeOperator("GlobalLpPool"),
    m_P(pP) {
}

void GlobalLpPool::print(std::ostream& pOS) const
{
}
