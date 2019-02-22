//===- NvDlaConvRelu.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaConvRelu.h"

using namespace onnc;

char NvDlaConvRelu::ID = 0;

//===----------------------------------------------------------------------===//
// ConvRelu
//===----------------------------------------------------------------------===//
void NvDlaConvRelu::printAttributes(std::ostream& pOS) const
{
  m_Conv.printAttributes(pOS);
  m_Relu.printAttributes(pOS);
}

void NvDlaConvRelu::accept(ComputeVisitor& pV)
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void NvDlaConvRelu::accept(ComputeVisitor& pV) const
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool NvDlaConvRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
