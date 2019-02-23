//===- NvDlaConvReluMaxPool.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaConvReluMaxPool.h"
#include "../NvDlaComputeVisitor.h"

using namespace onnc;

char NvDlaConvReluMaxPool::ID = 0;

//===----------------------------------------------------------------------===//
// NvDlaConvReluMaxPool
//===----------------------------------------------------------------------===//
void NvDlaConvReluMaxPool::printAttributes(std::ostream& pOS) const
{
  m_Conv.printAttributes(pOS);
  m_Relu.printAttributes(pOS);
  m_MaxPool.printAttributes(pOS);
}

void NvDlaConvReluMaxPool::accept(ComputeVisitor& pV)
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void NvDlaConvReluMaxPool::accept(ComputeVisitor& pV) const
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool NvDlaConvReluMaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
