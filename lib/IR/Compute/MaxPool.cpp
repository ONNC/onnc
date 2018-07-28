//===- MaxPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MaxPool.h>

using namespace onnc;

char MaxPool::ID = 0;

//===----------------------------------------------------------------------===//
// MaxPool
//===----------------------------------------------------------------------===//
MaxPool::MaxPool(const IntsAttr& pKernelShape)
  : ComputeOperator("MaxPool", ID),
    m_AutoPad(),
    m_KernelShape(pKernelShape),
    m_Pads(),
    m_Strides() {
}

MaxPool::MaxPool(const StringAttr& pAutoPad,
                 const IntsAttr& pKernelShape,
                 const IntsAttr& pPads,
                 const IntsAttr& pStrides)
  : ComputeOperator("MaxPool", ID),
    m_AutoPad(pAutoPad),
    m_KernelShape(pKernelShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

void MaxPool::print(std::ostream& pOS) const
{
}

bool MaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
