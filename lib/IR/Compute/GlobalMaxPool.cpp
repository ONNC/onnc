//===- GlobalMaxPool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalMaxPool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GlobalMaxPool
//===----------------------------------------------------------------------===//
GlobalMaxPool::GlobalMaxPool()
  : ComputeOperator("GlobalMaxPool") {
}



void GlobalMaxPool::print(std::ostream& pOS) const
{
}
