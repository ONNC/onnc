//===- X86Interpreter.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_X86_INTERPRETER_H
#define ONNC_X86_INTERPRETER_H
#include <onnc/Runtime/Interpreter.h>
#include "Compute/X86ComputeVisitor.h"
#include "Compute/X86ConvRelu.h"

namespace onnc {

template<typename OperatorVisitorT = X86ComputeVisitor>
class X86InterpreterVisitor : public InterpreterVisitor<OperatorVisitorT>
{
public:
  using Base = InterpreterVisitor<OperatorVisitorT>;
  void visit(X86ConvRelu& pX86ConvRelu) override {
    Base::visit(pX86ConvRelu.m_Conv);
    Base::visit(pX86ConvRelu.m_Relu);
  }
};

class X86Interpreter : public Interpreter
{
public:
  X86Interpreter()
      : Interpreter(new X86InterpreterVisitor<>()) {}

  // XXX(a127a127):
  //   Temporary hack, should use something like "Context" or "Environment",
  //   should not expose Interpreter visitor implementation.
  BasicInterpreter *getBasicInterpreter() override {
    return static_cast<X86InterpreterVisitor<>*>(&getVisitor());
  }
};

} // namespace of onnc

#endif
