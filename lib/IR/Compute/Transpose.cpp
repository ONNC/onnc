//===- Transpose.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Transpose.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Transpose
//===----------------------------------------------------------------------===//
Transpose::Transpose()
  : ComputeOperator("Transpose"),
    m_Perm() {
}

Transpose::Transpose(const IntsAttr& pPerm)
  : ComputeOperator("Transpose"),
    m_Perm(pPerm) {
}

void Transpose::print(std::ostream& pOS) const
{
}
