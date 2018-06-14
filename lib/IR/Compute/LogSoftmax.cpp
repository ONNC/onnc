//===- LogSoftmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LogSoftmax.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LogSoftmax
//===----------------------------------------------------------------------===//
LogSoftmax::LogSoftmax()
  : ComputeOperator("LogSoftmax"),
    m_Axis() {
}

LogSoftmax::LogSoftmax(const IntAttr& pAxis)
  : ComputeOperator("LogSoftmax"),
    m_Axis(pAxis) {
}

void LogSoftmax::print(std::ostream& pOS) const
{
}
