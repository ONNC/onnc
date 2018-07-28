//===- Scale.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Scale.h>

using namespace onnc;

char Scale::ID = 0;

//===----------------------------------------------------------------------===//
// Scale
//===----------------------------------------------------------------------===//
Scale::Scale()
  : ComputeOperator("Scale", ID),
    m_Scale() {
}

Scale::Scale(const FloatAttr& pScale)
  : ComputeOperator("Scale", ID),
    m_Scale(pScale) {
}

void Scale::print(std::ostream& pOS) const
{
}

bool Scale::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
