//===- MaxPool.cpp --------------------------------------------------------===//
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
    m_AutoPad("NOTSET"),
    m_KernelShape(pKernelShape),
    m_Pads(),
    m_StorageOrder(0),
    m_Strides() {
}

MaxPool::MaxPool(const StringAttr& pAutoPad,
                 const IntsAttr& pKernelShape,
                 const IntsAttr& pPads,
                 const IntAttr& pStorageOrder,
                 const IntsAttr& pStrides)
  : ComputeOperator("MaxPool", ID),
    m_AutoPad(pAutoPad),
    m_KernelShape(pKernelShape),
    m_Pads(pPads),
    m_StorageOrder(pStorageOrder),
    m_Strides(pStrides) {
}

MaxPool::MaxPool(const MaxPool& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AutoPad(pCopy.getAutoPad()),
    m_KernelShape(pCopy.getKernelShape()),
    m_Pads(pCopy.getPads()),
    m_StorageOrder(pCopy.getStorageOrder()),
    m_Strides(pCopy.getStrides()) {
}

void MaxPool::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "auto_pad: " << getAutoPad() << ", " "kernel_shape: " << getKernelShape() << ", " "pads: " << getPads() << ", " "storage_order: " << getStorageOrder() << ", " "strides: " << getStrides()<< '>';
}

bool MaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
