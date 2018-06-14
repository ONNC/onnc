//===- Softplus.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softplus.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Softplus
//===----------------------------------------------------------------------===//
Softplus::Softplus()
  : ComputeOperator("Softplus") {
}



void Softplus::print(std::ostream& pOS) const
{
}
