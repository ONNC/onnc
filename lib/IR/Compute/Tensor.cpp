//===- Tensor.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tensor.h>

//===----------------------------------------------------------------------===//
// Tensor
//===----------------------------------------------------------------------===//
namespace onnc {

Tensor::Tensor()
  : onnc::Value() {
}

Tensor::Tensor(onnc::Value::Type pKind)
  : onnc::Value(pKind) {
}

Tensor::Tensor(const std::string& pName, onnc::Value::Type pKind)
  : onnc::Value(pName, pKind) {
}

Tensor::Tensor(onnc::Value::Type pKind, xTensor& pAdaptee)
  : onnc::Value(pKind, pAdaptee) {
}

void Tensor::print(std::ostream& pOS) const {
  // Print value name, e.g. "%data0"
  pOS << '%' << getName();
  // Print value type, e.g. "<float>"
  pOS << '<';
  switch (kind()) {
    case kFloat: pOS << "float"; break;
    case kUint8: pOS << "uint8"; break;
    case kInt8: pOS << "int8"; break;
    case kUint16: pOS << "uint16"; break;
    case kInt16: pOS << "int16"; break;
    case kInt32: pOS << "int32"; break;
    case kInt64: pOS << "int64"; break;
    case kString: pOS << "string"; break;
    case kBoolean: pOS << "bool"; break;

    case kFloat16: pOS << "float16"; break;
    case kDouble: pOS << "double"; break;
    case kUint32: pOS << "uint32"; break;
    case kUint64: pOS << "uint64"; break;

    case kComplex64: pOS << "complex64"; break;
    case kComplex128: pOS << "complex128"; break;
    default: pOS << "unknown"; break;
  }
  pOS << '>';
  // Print dimensions, e.g. "[1, 3, 244, 244]"
  pOS << '[';
  for (int i = 0; i < m_Dimensions.size(); ++i) {
    if (i > 0) pOS << ", ";
    pOS << m_Dimensions[i];
  }
  pOS << ']';
}

Tensor::Size size(const Tensor& tensor)
{
  Tensor::Size size = 1;
  for (Tensor::Size idx = 0; idx < tensor.getNumOfDimensions(); ++idx) {
     size *= static_cast<Tensor::Size>(tensor.dimension(idx));
  }

  return size;
}

} // namespace onnc
