//===- LogSoftmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LogSoftmax.h>

using namespace onnc;

char LogSoftmax::ID = 0;

//===----------------------------------------------------------------------===//
// LogSoftmax
//===----------------------------------------------------------------------===//
LogSoftmax::LogSoftmax()
  : ComputeOperator("LogSoftmax", ID),
    m_Axis() {
}

LogSoftmax::LogSoftmax(const IntAttr& pAxis)
  : ComputeOperator("LogSoftmax", ID),
    m_Axis(pAxis) {
}

void LogSoftmax::print(std::ostream& pOS) const
{
}

bool LogSoftmax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
