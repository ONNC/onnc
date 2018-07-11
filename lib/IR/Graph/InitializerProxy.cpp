//===- InitializerProxy.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Graph/InitializerProxy.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// InitializerProxy
//===----------------------------------------------------------------------===//
InitializerProxy::InitializerProxy()
  : m_pName(nullptr), m_pTensor(nullptr) {
}

InitializerProxy::InitializerProxy(const std::string& pName,
                         const ::onnx::Tensor& pTensor)
  : m_pName(&pName), m_pTensor(&pTensor) {
}
