//===- Relu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Relu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Relu
//===----------------------------------------------------------------------===//
Relu::Relu()
  : ComputeOperator("Relu") {
}



void Relu::print(std::ostream& pOS) const
{
}
