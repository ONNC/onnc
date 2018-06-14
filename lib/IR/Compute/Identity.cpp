//===- Identity.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Identity.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Identity
//===----------------------------------------------------------------------===//
Identity::Identity()
  : ComputeOperator("Identity") {
}



void Identity::print(std::ostream& pOS) const
{
}
