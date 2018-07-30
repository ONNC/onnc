//===- Clip.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Clip.h>

using namespace onnc;

char Clip::ID = 0;

//===----------------------------------------------------------------------===//
// Clip
//===----------------------------------------------------------------------===//
Clip::Clip()
  : ComputeOperator("Clip", ID),
    m_Max(3.40282346639e+38),
    m_Min(-3.40282346639e+38) {
}

Clip::Clip(const FloatAttr& pMax,
           const FloatAttr& pMin)
  : ComputeOperator("Clip", ID),
    m_Max(pMax),
    m_Min(pMin) {
}

Clip::Clip(const Clip& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Max(pCopy.getMax()),
    m_Min(pCopy.getMin()) {
}

void Clip::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getMax() << ", " << getMin() << ">";
}

bool Clip::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
