//===- MatMul.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MatMul.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MatMul
//===----------------------------------------------------------------------===//
MatMul::MatMul()
  : ComputeOperator("MatMul") {
}



void MatMul::print(std::ostream& pOS) const
{
}
