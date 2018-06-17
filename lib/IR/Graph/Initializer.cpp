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
Initializer::Initializer(std::string& pName, ::onnx::Tensor& pTensor)
  : m_Name(pName), m_Tensor(pTensor) {
}
