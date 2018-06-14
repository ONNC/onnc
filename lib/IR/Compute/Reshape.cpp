//===- Reshape.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Reshape.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Reshape
//===----------------------------------------------------------------------===//
Reshape::Reshape()
  : ComputeOperator("Reshape") {
}



void Reshape::print(std::ostream& pOS) const
{
}
