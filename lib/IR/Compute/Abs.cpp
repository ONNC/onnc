//===- Abs.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Abs.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Abs
//===----------------------------------------------------------------------===//
Abs::Abs()
  : ComputeOperator("Abs") {
}

Abs::Abs(const Abs& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Abs::print(std::ostream& pOS) const
{
  pOS << name();
}
