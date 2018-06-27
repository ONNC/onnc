//===- Acos.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Acos.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Acos
//===----------------------------------------------------------------------===//
Acos::Acos()
  : ComputeOperator("Acos") {
}

Acos::Acos(const Acos& pCopy)
  : ComputeOperator(pCopy) {
}

void Acos::print(std::ostream& pOS) const
{
  pOS << name();
}
