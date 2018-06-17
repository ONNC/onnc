//===- Initializer.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_INITIALIZER_H
#define ONNC_IR_INITIALIZER_H
#include <ostream>
#include <onnx/common/ir.h>
#include <vector>

namespace onnc {

/** \class Initializer
 *
 *  There is no concrete Initializer class in ::onnx::graph.
 *  Initializer is a proxy to encapsulate ::onnx::Graph's initializer.
 */
class Initializer
{
public:
  Initializer(std::string& pName, ::onnx::Tensor& pTensor);

  ~Initializer() { }

  const std::string& name() const { return m_Name; }

  ::onnx::Tensor& tensor() { return m_Tensor; }

  const ::onnx::Tensor& tensor() const { return m_Tensor; }

private:
  std::string& m_Name;
  ::onnx::Tensor& m_Tensor;
};

} // namespace of onnc

#endif
