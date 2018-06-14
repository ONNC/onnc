//===- GlobalAveragePool.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GlobalAveragePool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GlobalAveragePool
//===----------------------------------------------------------------------===//
GlobalAveragePool::GlobalAveragePool()
  : ComputeOperator("GlobalAveragePool") {
}



void GlobalAveragePool::print(std::ostream& pOS) const
{
}
