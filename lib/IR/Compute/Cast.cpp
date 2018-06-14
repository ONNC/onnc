//===- Cast.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Cast.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Cast
//===----------------------------------------------------------------------===//
Cast::Cast()
  : ComputeOperator("Cast"),
    m_To() {
}

Cast::Cast(const IntAttr& pTo)
  : ComputeOperator("Cast"),
    m_To(pTo) {
}

void Cast::print(std::ostream& pOS) const
{
}
