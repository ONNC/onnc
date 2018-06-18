//===- Scalar.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Scalar.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Scalar
//===----------------------------------------------------------------------===//
Scalar::Scalar()
  : onnc::Value(onnc::Value::kUndefined) {
}

Scalar::Scalar(onnc::Value::Type pKind)
  : onnc::Value(pKind) {
}

Scalar::Scalar(const std::string& pName, onnc::Value::Type pKind)
  : onnc::Value(pName, pKind) {
}

Scalar::Scalar(onnc::Value::Type pKind, ::onnx::Tensor& pAdaptee)
  : onnc::Value(pKind, pAdaptee) {
}
