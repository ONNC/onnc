//===- Tanh.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tanh.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Tanh
//===----------------------------------------------------------------------===//
Tanh::Tanh()
  : ComputeOperator("Tanh") {
}



void Tanh::print(std::ostream& pOS) const
{
}
