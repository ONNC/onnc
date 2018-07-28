//===- GRUUnit.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GRUUnit.h>

using namespace onnc;

char GRUUnit::ID = 0;

//===----------------------------------------------------------------------===//
// GRUUnit
//===----------------------------------------------------------------------===//
GRUUnit::GRUUnit()
  : ComputeOperator("GRUUnit", ID),
    m_DropStates() {
}

GRUUnit::GRUUnit(const IntAttr& pDropStates)
  : ComputeOperator("GRUUnit", ID),
    m_DropStates(pDropStates) {
}

void GRUUnit::print(std::ostream& pOS) const
{
}

bool GRUUnit::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
