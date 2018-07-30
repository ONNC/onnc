//===- Size.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Size.h>

using namespace onnc;

char Size::ID = 0;

//===----------------------------------------------------------------------===//
// Size
//===----------------------------------------------------------------------===//
Size::Size()
  : ComputeOperator("Size", ID) {
}



Size::Size(const Size& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Size::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Size::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
