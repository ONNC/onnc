//===- InitializerProxy.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Tensor/InitializerProxy.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// InitializerProxy
//===----------------------------------------------------------------------===//
InitializerProxy::InitializerProxy()
  : m_pName(nullptr), m_pTensor(nullptr) {
}

InitializerProxy::InitializerProxy(const std::string& pName,
                                   const xTensor& pTensor)
  : m_pName(&pName), m_pTensor(&pTensor) {
}
