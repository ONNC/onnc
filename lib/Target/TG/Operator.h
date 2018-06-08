#pragma once

#include <onnx/common/ir.h>
#include <string>

namespace onnc {

using MemTable = std::map<std::string, uint64_t>;
class Operator
{
public:
  Operator(const ::onnx::Node &pNode, const std::string &pTypeName)
      : m_TypeName(pTypeName)
  {
    m_LayerName = const_cast< ::onnx::Node &>(pNode).output()->uniqueName();
  }

  virtual ~Operator() = default;

  // FIXME changed to use ENUM when compute ir finish
  const std::string &getTypeName() { return m_TypeName; }

  const std::string &getLayerName() { return m_LayerName; };

  virtual void emit(void) const = 0;

private:
  std::string m_TypeName;
  std::string m_LayerName;
};

} // namespace onnc
