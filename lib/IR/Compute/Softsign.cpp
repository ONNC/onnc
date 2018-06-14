//===- Softsign.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softsign.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Softsign
//===----------------------------------------------------------------------===//
Softsign::Softsign()
  : ComputeOperator("Softsign") {
}



void Softsign::print(std::ostream& pOS) const
{
}
