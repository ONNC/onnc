#pragma once

#include <onnx/common/ir.h>
#include <string>

namespace onnc {

class Operator
{
public:
  Operator(const ::onnx::Node &pNode, const std::string &pName) : m_name(pName)
  {
  }

  virtual ~Operator() { }

  const std::string &getName(void) const { return m_name; }

  virtual void emit(void) const = 0;

private:
  std::string m_name;
};

} // namespace onnc
