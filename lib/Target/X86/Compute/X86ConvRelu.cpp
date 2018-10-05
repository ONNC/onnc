//===- X86ConvRelu.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86ConvRelu.h"

using namespace onnc;

char X86ConvRelu::ID = 0;

//===----------------------------------------------------------------------===//
// X86ConvRelu
//===----------------------------------------------------------------------===//
void X86ConvRelu::printAttributes(std::ostream& pOS) const
{
  // m_Conv->printAttributes(pOS);
  // m_Relu->printAttributes(pOS);
}

void X86ConvRelu::accept(ComputeVisitor &pV)
{
  // X86Visitor* visitor = dyn_cast<X86Visitor>(&pV);
  // if (nullptr != visitor)
  //   visitor->visit(*this);
}

void X86ConvRelu::accept(ComputeVisitor &pV) const
{
  // X86Visitor* visitor = dyn_cast<X86Visitor>(&pV);
  // if (nullptr != visitor)
  //  visitor->visit(*this);
}

bool X86ConvRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
