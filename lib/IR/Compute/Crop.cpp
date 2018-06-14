//===- Crop.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Crop.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Crop
//===----------------------------------------------------------------------===//
Crop::Crop()
  : ComputeOperator("Crop"),
    m_Border(),
    m_Scale() {
}

Crop::Crop(const IntsAttr& pBorder,
           const IntsAttr& pScale)
  : ComputeOperator("Crop"),
    m_Border(pBorder),
    m_Scale(pScale) {
}

void Crop::print(std::ostream& pOS) const
{
}
