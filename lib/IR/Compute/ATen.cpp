//===- ATen.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ATen.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ATen
//===----------------------------------------------------------------------===//
ATen::ATen()
  : ComputeOperator("ATen") {
}

ATen::ATen(const ATen& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void ATen::print(std::ostream& pOS) const
{
  pOS << name();
}
