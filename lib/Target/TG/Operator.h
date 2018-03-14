#pragma once

#include <onnx/common/ir.h>
#include <string>

namespace onnc {

class Operator {
public:
  Operator(const onnx::Node &node, const std::string &name) {};
  virtual ~Operator() {};
  const std::string &getName(void) { return m_name; }
  virtual void emit(void) const = 0;

private:
  std::string m_name;
};

} // namespace onnc
