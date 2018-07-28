//===- Concat.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Concat.h>

using namespace onnc;

char Concat::ID = 0;

//===----------------------------------------------------------------------===//
// Concat
//===----------------------------------------------------------------------===//
Concat::Concat(const IntAttr& pAxis)
  : ComputeOperator("Concat", ID),
    m_Axis(pAxis) {
}

void Concat::print(std::ostream& pOS) const
{
}

bool Concat::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
