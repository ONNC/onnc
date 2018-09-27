//===- Squeeze.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Squeeze.h>

using namespace onnc;

char Squeeze::ID = 0;

//===----------------------------------------------------------------------===//
// Squeeze
//===----------------------------------------------------------------------===//
Squeeze::Squeeze()
  : ComputeOperator("Squeeze", ID),
    m_Axes() {
}

Squeeze::Squeeze(const IntsAttr& pAxes)
  : ComputeOperator("Squeeze", ID),
    m_Axes(pAxes) {
}

Squeeze::Squeeze(const Squeeze& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()) {
}

void Squeeze::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "axes: " << getAxes()<< '>';
}

bool Squeeze::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
