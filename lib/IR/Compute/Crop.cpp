//===- Crop.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Crop.h>

using namespace onnc;

char Crop::ID = 0;

//===----------------------------------------------------------------------===//
// Crop
//===----------------------------------------------------------------------===//
Crop::Crop()
  : ComputeOperator("Crop", ID),
    m_Border(),
    m_Scale() {
}

Crop::Crop(const IntsAttr& pBorder,
           const IntsAttr& pScale)
  : ComputeOperator("Crop", ID),
    m_Border(pBorder),
    m_Scale(pScale) {
}

Crop::Crop(const Crop& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Border(pCopy.getBorder()),
    m_Scale(pCopy.getScale()) {
}

void Crop::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getBorder() << ", " << getScale() << ">";
}

bool Crop::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
