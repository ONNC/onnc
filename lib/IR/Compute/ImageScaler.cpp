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
    m_Scale(1.0) {
}

ImageScaler::ImageScaler(const FloatsAttr& pBias,
                         const FloatAttr& pScale)
  : ComputeOperator("ImageScaler", ID),
    m_Bias(pBias),
    m_Scale(pScale) {
}

ImageScaler::ImageScaler(const ImageScaler& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Bias(pCopy.getBias()),
    m_Scale(pCopy.getScale()) {
}

void ImageScaler::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getBias() << ", " << getScale() << ">";
}

bool ImageScaler::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
