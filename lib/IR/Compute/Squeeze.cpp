//===- Squeeze.cpp ------------------------------------------------===//
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

void Squeeze::print(std::ostream& pOS) const
{
}

bool Squeeze::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
