//===- Log.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Log.h>

using namespace onnc;

char Log::ID = 0;

//===----------------------------------------------------------------------===//
// Log
//===----------------------------------------------------------------------===//
Log::Log()
  : ComputeOperator("Log", ID) {
}



void Log::print(std::ostream& pOS) const
{
}

bool Log::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
