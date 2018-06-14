//===- ImageScaler.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ImageScaler.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ImageScaler
//===----------------------------------------------------------------------===//
ImageScaler::ImageScaler()
  : ComputeOperator("ImageScaler"),
    m_Bias(),
    m_Scale() {
}

ImageScaler::ImageScaler(const FloatsAttr& pBias,
                         const FloatAttr& pScale)
  : ComputeOperator("ImageScaler"),
    m_Bias(pBias),
    m_Scale(pScale) {
}

void ImageScaler::print(std::ostream& pOS) const
{
}
