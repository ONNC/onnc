//===- Cast.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Cast.h>

using namespace onnc;

char Cast::ID = 0;

//===----------------------------------------------------------------------===//
// Cast
//===----------------------------------------------------------------------===//
Cast::Cast()
  : ComputeOperator("Cast", ID),
    m_To() {
}

Cast::Cast(const IntAttr& pTo)
  : ComputeOperator("Cast", ID),
    m_To(pTo) {
}

void Cast::print(std::ostream& pOS) const
{
}

bool Cast::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
