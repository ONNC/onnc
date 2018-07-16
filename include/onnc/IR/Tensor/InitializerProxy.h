//===- InitializerProxy.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_INITIALIZER_PROXY_H
#define ONNC_IR_INITIALIZER_PROXY_H
#include <ostream>
#include <onnx/common/ir.h>
#include <string>
#include <vector>

namespace onnc {

/** \class InitializerProxy
 *
 *  There is no concrete InitializerProxy class in ::onnx::graph.
 *  InitializerProxy is a proxy to encapsulate ::onnx::Graph's initializer.
 */
class InitializerProxy
{
public:
  InitializerProxy();

  InitializerProxy(const std::string& pName, const ::onnx::Tensor& pTensor);

  ~InitializerProxy() { }

  void setName(const std::string& pName) { m_pName = &pName; }

  const std::string& name() const { return *m_pName; }

  void setTensor(const ::onnx::Tensor& pTensor) { m_pTensor = &pTensor; }

  const ::onnx::Tensor* tensor() const { return m_pTensor; }

  bool isValid() const { return (nullptr == m_pName || nullptr == m_pTensor); }

private:
  const std::string* m_pName;
  const ::onnx::Tensor* m_pTensor;
};

} // namespace of onnc

#endif
