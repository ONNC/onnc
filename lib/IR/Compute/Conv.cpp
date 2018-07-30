//===- Conv.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Conv.h>

using namespace onnc;

char Conv::ID = 0;

//===----------------------------------------------------------------------===//
// Conv
//===----------------------------------------------------------------------===//
Conv::Conv()
  : ComputeOperator("Conv", ID),
    m_AutoPad("NOTSET"),
    m_Dilations(),
    m_Group(1),
    m_KernelShape(),
    m_Pads(),
    m_Strides() {
}

Conv::Conv(const StringAttr& pAutoPad,
           const IntsAttr& pDilations,
           const IntAttr& pGroup,
           const IntsAttr& pKernelShape,
           const IntsAttr& pPads,
           const IntsAttr& pStrides)
  : ComputeOperator("Conv", ID),
    m_AutoPad(pAutoPad),
    m_Dilations(pDilations),
    m_Group(pGroup),
    m_KernelShape(pKernelShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

Conv::Conv(const Conv& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AutoPad(pCopy.getAutoPad()),
    m_Dilations(pCopy.getDilations()),
    m_Group(pCopy.getGroup()),
    m_KernelShape(pCopy.getKernelShape()),
    m_Pads(pCopy.getPads()),
    m_Strides(pCopy.getStrides()) {
}

void Conv::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAutoPad() << ", " << getDilations() << ", " << getGroup() << ", " << getKernelShape() << ", " << getPads() << ", " << getStrides() << ">";
}

bool Conv::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
