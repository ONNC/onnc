//===- ConvTranspose.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ConvTranspose.h>

using namespace onnc;

char ConvTranspose::ID = 0;

//===----------------------------------------------------------------------===//
// ConvTranspose
//===----------------------------------------------------------------------===//
ConvTranspose::ConvTranspose()
  : ComputeOperator("ConvTranspose", ID),
    m_AutoPad("NOTSET"),
    m_Dilations(),
    m_Group(1),
    m_KernelShape(),
    m_OutputPadding(),
    m_OutputShape(),
    m_Pads(),
    m_Strides() {
}

ConvTranspose::ConvTranspose(const StringAttr& pAutoPad,
                             const IntsAttr& pDilations,
                             const IntAttr& pGroup,
                             const IntsAttr& pKernelShape,
                             const IntsAttr& pOutputPadding,
                             const IntsAttr& pOutputShape,
                             const IntsAttr& pPads,
                             const IntsAttr& pStrides)
  : ComputeOperator("ConvTranspose", ID),
    m_AutoPad(pAutoPad),
    m_Dilations(pDilations),
    m_Group(pGroup),
    m_KernelShape(pKernelShape),
    m_OutputPadding(pOutputPadding),
    m_OutputShape(pOutputShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

ConvTranspose::ConvTranspose(const ConvTranspose& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AutoPad(pCopy.getAutoPad()),
    m_Dilations(pCopy.getDilations()),
    m_Group(pCopy.getGroup()),
    m_KernelShape(pCopy.getKernelShape()),
    m_OutputPadding(pCopy.getOutputPadding()),
    m_OutputShape(pCopy.getOutputShape()),
    m_Pads(pCopy.getPads()),
    m_Strides(pCopy.getStrides()) {
}

void ConvTranspose::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAutoPad() << ", " << getDilations() << ", " << getGroup() << ", " << getKernelShape() << ", " << getOutputPadding() << ", " << getOutputShape() << ", " << getPads() << ", " << getStrides() << ">";
}

bool ConvTranspose::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
