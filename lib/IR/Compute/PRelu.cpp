//===- PRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/PRelu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PRelu
//===----------------------------------------------------------------------===//
PRelu::PRelu()
  : ComputeOperator("PRelu") {
}



void PRelu::print(std::ostream& pOS) const
{
}
