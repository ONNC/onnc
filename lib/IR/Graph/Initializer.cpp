//===- Initializer.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Graph/Initializer.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Initializer
//===----------------------------------------------------------------------===//
Initializer::Initializer()
  : m_pName(nullptr), m_pTensor(nullptr) {
}

Initializer::Initializer(const std::string& pName,
                         const ::onnx::Tensor& pTensor)
  : m_pName(&pName), m_pTensor(&pTensor) {
}
