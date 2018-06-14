//===- Sigmoid.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sigmoid.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Sigmoid
//===----------------------------------------------------------------------===//
Sigmoid::Sigmoid()
  : ComputeOperator("Sigmoid") {
}



void Sigmoid::print(std::ostream& pOS) const
{
}
