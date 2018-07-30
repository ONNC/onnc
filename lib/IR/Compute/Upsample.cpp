//===- Upsample.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Upsample.h>

using namespace onnc;

char Upsample::ID = 0;

//===----------------------------------------------------------------------===//
// Upsample
//===----------------------------------------------------------------------===//
Upsample::Upsample(const FloatsAttr& pScales)
  : ComputeOperator("Upsample", ID),
    m_Mode("nearest"),
    m_Scales(pScales) {
}

Upsample::Upsample(const StringAttr& pMode,
                   const FloatsAttr& pScales)
  : ComputeOperator("Upsample", ID),
    m_Mode(pMode),
    m_Scales(pScales) {
}

Upsample::Upsample(const Upsample& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Mode(pCopy.getMode()),
    m_Scales(pCopy.getScales()) {
}

void Upsample::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getMode() << ", " << getScales() << ">";
}

bool Upsample::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
