//===- ImageScaler.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ImageScaler.h>

using namespace onnc;

char ImageScaler::ID = 0;

//===----------------------------------------------------------------------===//
// ImageScaler
//===----------------------------------------------------------------------===//
ImageScaler::ImageScaler()
  : ComputeOperator("ImageScaler", ID),
    m_Bias(),
    m_Scale() {
}

ImageScaler::ImageScaler(const FloatsAttr& pBias,
                         const FloatAttr& pScale)
  : ComputeOperator("ImageScaler", ID),
    m_Bias(pBias),
    m_Scale(pScale) {
}

void ImageScaler::print(std::ostream& pOS) const
{
}

bool ImageScaler::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
