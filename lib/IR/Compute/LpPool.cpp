//===- LpPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LpPool.h>

using namespace onnc;

char LpPool::ID = 0;

//===----------------------------------------------------------------------===//
// LpPool
//===----------------------------------------------------------------------===//
LpPool::LpPool(const IntsAttr& pKernelShape)
  : ComputeOperator("LpPool", ID),
    m_AutoPad("NOTSET"),
    m_KernelShape(pKernelShape),
    m_P(2),
    m_Pads(),
    m_Strides() {
}

LpPool::LpPool(const StringAttr& pAutoPad,
               const IntsAttr& pKernelShape,
               const IntAttr& pP,
               const IntsAttr& pPads,
               const IntsAttr& pStrides)
  : ComputeOperator("LpPool", ID),
    m_AutoPad(pAutoPad),
    m_KernelShape(pKernelShape),
    m_P(pP),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

LpPool::LpPool(const LpPool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AutoPad(pCopy.getAutoPad()),
    m_KernelShape(pCopy.getKernelShape()),
    m_P(pCopy.getP()),
    m_Pads(pCopy.getPads()),
    m_Strides(pCopy.getStrides()) {
}

void LpPool::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAutoPad() << ", " << getKernelShape() << ", " << getP() << ", " << getPads() << ", " << getStrides() << ">";
}

bool LpPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
