//===- Add.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Add.h>

using namespace onnc;

char Add::ID = 0;

//===----------------------------------------------------------------------===//
// Add
//===----------------------------------------------------------------------===//
Add::Add()
  : ComputeOperator("Add", ID) {
}



Add::Add(const Add& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Add::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Add::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
