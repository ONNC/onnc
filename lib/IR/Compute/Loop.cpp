//===- Loop.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Loop.h>

using namespace onnc;

char Loop::ID = 0;

//===----------------------------------------------------------------------===//
// Loop
//===----------------------------------------------------------------------===//
Loop::Loop()
  : ComputeOperator("Loop", ID),
    m_Body() {
}

Loop::Loop(const GraphAttr& pBody)
  : ComputeOperator("Loop", ID),
    m_Body(pBody) {
}

void Loop::print(std::ostream& pOS) const
{
}

bool Loop::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
