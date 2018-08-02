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
    m_AutoPad("NOTSET"),
    m_CountIncludePad(0),
    m_KernelShape(pKernelShape),
    m_Pads(),
    m_Strides() {
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

AveragePool::AveragePool(const AveragePool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AutoPad(pCopy.getAutoPad()),
    m_CountIncludePad(pCopy.getCountIncludePad()),
    m_KernelShape(pCopy.getKernelShape()),
    m_Pads(pCopy.getPads()),
    m_Strides(pCopy.getStrides()) {
}

void AveragePool::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAutoPad() << ", " << getCountIncludePad() << ", " << getKernelShape() << ", " << getPads() << ", " << getStrides() << ">";
}

bool AveragePool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
