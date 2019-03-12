//===- NvDlaMaxPool.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaMaxPool.h"

#include "../NvDlaComputeVisitor.h"

using namespace onnc;

char NvDlaMaxPool::ID = 0;

//===----------------------------------------------------------------------===//
// ConvRelu
//===----------------------------------------------------------------------===//
void NvDlaMaxPool::printAttributes(std::ostream& pOS) const
{
  m_MaxPool.printAttributes(pOS);
  pOS << "<group: " << m_Group << ">";
}

void NvDlaMaxPool::accept(ComputeVisitor& pV)
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void NvDlaMaxPool::accept(ComputeVisitor& pV) const
{
  NvDlaComputeVisitor* visitor = dyn_cast<NvDlaComputeVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool NvDlaMaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
