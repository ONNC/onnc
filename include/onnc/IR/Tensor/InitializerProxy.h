//===- InitializerProxy.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_INITIALIZER_PROXY_H
#define ONNC_IR_INITIALIZER_PROXY_H
#include <onnc/Config/ONNX.h>
#include <ostream>
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

  InitializerProxy(const std::string& pName, const xTensor& pTensor);

  ~InitializerProxy() { }

  void setName(const std::string& pName) { m_pName = &pName; }

  const std::string& name() const { return *m_pName; }

  void setTensor(const xTensor& pTensor) { m_pTensor = &pTensor; }

  const xTensor* tensor() const { return m_pTensor; }

  bool isValid() const { return (nullptr == m_pName || nullptr == m_pTensor); }

private:
  const std::string* m_pName;
  const xTensor* m_pTensor;
};

} // namespace of onnc

#endif
