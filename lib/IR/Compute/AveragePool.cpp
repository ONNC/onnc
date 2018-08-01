//===- AveragePool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/AveragePool.h>

using namespace onnc;

char AveragePool::ID = 0;

//===----------------------------------------------------------------------===//
// AveragePool
//===----------------------------------------------------------------------===//
AveragePool::AveragePool(const IntsAttr& pKernelShape)
  : ComputeOperator("AveragePool", ID),
    m_AutoPad(),
    m_CountIncludePad(),
    m_KernelShape(pKernelShape),
    m_Pads(IntsAttr(4, 0)), //< fill constructor {0, 0, 0, 0}
    m_Strides(IntsAttr(2, 1)) { //< fill constructor {1, 1}
}

AveragePool::AveragePool(const StringAttr& pAutoPad,
                         const IntAttr& pCountIncludePad,
                         const IntsAttr& pKernelShape,
                         const IntsAttr& pPads,
                         const IntsAttr& pStrides)
  : ComputeOperator("AveragePool", ID),
    m_AutoPad(pAutoPad),
    m_CountIncludePad(pCountIncludePad),
    m_KernelShape(pKernelShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

void AveragePool::print(std::ostream& pOS) const
{
}

bool AveragePool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
