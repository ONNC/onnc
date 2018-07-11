//===- Initializer.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Initializer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Initializer
//===----------------------------------------------------------------------===//
Initializer::Initializer()
  : ComputeOperator("Initializer"),
    m_Name() {
}

Initializer::Initializer(const StringAttr& pName)
  : ComputeOperator("Initializer"),
    m_Name(pName) {
}

Initializer::~Initializer()
{
}

void Initializer::print(std::ostream& pOS) const
{
  pOS << name();
}
