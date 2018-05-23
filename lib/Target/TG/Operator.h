#pragma once

#include <onnx/common/ir.h>
#include <string>

namespace onnc {

using MemTable = std::map<std::string, uint64_t>;
class Operator
{
public:
  Operator(const ::onnx::Node &pNode, const std::string &pName) : m_name(pName)
  {
  }

  virtual ~Operator() = default;

  const std::string &getName(void) const { return m_name; }

  virtual void emit(void) const = 0;

private:
  std::string m_name;
};

} // namespace onnc
