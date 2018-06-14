//===- GRUUnit.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GRUUnit.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GRUUnit
//===----------------------------------------------------------------------===//
GRUUnit::GRUUnit()
  : ComputeOperator("GRUUnit"),
    m_DropStates() {
}

GRUUnit::GRUUnit(const IntAttr& pDropStates)
  : ComputeOperator("GRUUnit"),
    m_DropStates(pDropStates) {
}

void GRUUnit::print(std::ostream& pOS) const
{
}
