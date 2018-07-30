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
    m_Scale(1.0) {
}

Scale::Scale(const FloatAttr& pScale)
  : ComputeOperator("Scale", ID),
    m_Scale(pScale) {
}

Scale::Scale(const Scale& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Scale(pCopy.getScale()) {
}

void Scale::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getScale() << ">";
}

bool Scale::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
