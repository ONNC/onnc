#include "Operator.h"
#include "oper.h"

#pragma once

namespace onnc {
namespace tensor {

class OperatorVisitor {
public:
  virtual void visit(Operator& oper);
  virtual void visit(Add& oper_add);
};

} // namespace tensor
} // namespace onnc
