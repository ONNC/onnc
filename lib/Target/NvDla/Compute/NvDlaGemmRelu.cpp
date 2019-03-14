//===- NvDlaGemmRelu.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaGemmRelu.h"

#include "../NvDlaComputeVisitor.h"

using namespace onnc;

char NvDlaGemmRelu::ID = 0;

//===----------------------------------------------------------------------===//
// GemmRelu
//===----------------------------------------------------------------------===//
void NvDlaGemmRelu::printAttributes(std::ostream& pOS) const
{
  m_Gemm.printAttributes(pOS);
  m_Relu.printAttributes(pOS);
}

void NvDlaGemmRelu::accept(ComputeVisitor& pV)
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void NvDlaGemmRelu::accept(ComputeVisitor& pV) const
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool NvDlaGemmRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
