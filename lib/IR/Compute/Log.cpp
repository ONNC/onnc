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



Log::Log(const Log& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Log::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Log::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
