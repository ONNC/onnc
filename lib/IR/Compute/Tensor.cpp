//===- Tensor.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tensor.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Tensor
//===----------------------------------------------------------------------===//
Tensor::Tensor()
  : onnc::Value() {
}

Tensor::Tensor(onnc::Value::Type pKind)
  : onnc::Value(pKind) {
}

Tensor::Tensor(const std::string& pName, onnc::Value::Type pKind)
  : onnc::Value(pName, pKind) {
}

Tensor::Tensor(onnc::Value::Type pKind, ::onnx::Tensor& pAdaptee)
  : onnc::Value(pKind, pAdaptee) {
}
