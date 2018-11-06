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
/** \class Interpreter
 *  \brief Interpreter dispatch compute ir to runtime.
 */
class X86Interpreter : public Interpreter<X86ComputeVisitor>
{
  virtual void visit(X86ConvRelu& pX86ConvRelu) {
    InterpreterBase::visit(pX86ConvRelu.m_Conv);
    InterpreterBase::visit(pX86ConvRelu.m_Relu);
  }
};

} // namespace of onnc

#endif
