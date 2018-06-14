//===- Loop.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Loop.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Loop
//===----------------------------------------------------------------------===//
Loop::Loop()
  : ComputeOperator("Loop"),
    m_Body() {
}

Loop::Loop(const GraphAttr& pBody)
  : ComputeOperator("Loop"),
    m_Body(pBody) {
}

void Loop::print(std::ostream& pOS) const
{
}
