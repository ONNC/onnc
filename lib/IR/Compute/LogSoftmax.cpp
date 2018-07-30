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
    m_Axis(1) {
}

LogSoftmax::LogSoftmax(const IntAttr& pAxis)
  : ComputeOperator("LogSoftmax", ID),
    m_Axis(pAxis) {
}

LogSoftmax::LogSoftmax(const LogSoftmax& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()) {
}

void LogSoftmax::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ">";
}

bool LogSoftmax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
