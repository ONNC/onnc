//===- LpPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LpPool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LpPool
//===----------------------------------------------------------------------===//
LpPool::LpPool()
  : ComputeOperator("LpPool"),
    m_AutoPad(),
    m_KernelShape(),
    m_P(),
    m_Pads(),
    m_Strides() {
}

LpPool::LpPool(const StringAttr& pAutoPad,
               const IntsAttr& pKernelShape,
               const IntAttr& pP,
               const IntsAttr& pPads,
               const IntsAttr& pStrides)
  : ComputeOperator("LpPool"),
    m_AutoPad(pAutoPad),
    m_KernelShape(pKernelShape),
    m_P(pP),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

void LpPool::print(std::ostream& pOS) const
{
}
