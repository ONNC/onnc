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
    m_Max(),
    m_Min() {
}

Clip::Clip(const FloatAttr& pMax,
           const FloatAttr& pMin)
  : ComputeOperator("Clip", ID),
    m_Max(pMax),
    m_Min(pMin) {
}

void Clip::print(std::ostream& pOS) const
{
}

bool Clip::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
