//===- X86ComputeVisitor.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_X86_X86_COMPUTE_VISITOR_H
#define ONNC_TARGET_X86_X86_COMPUTE_VISITOR_H

#include <onnc/IR/ComputeVisitor.h>

namespace onnc {

class X86ConvRelu;

/** \class X86ComputeVisitor
 */
class X86ComputeVisitor : public ComputeVisitor
{
public:
  static char ID;

  X86ComputeVisitor() : ComputeVisitor(ID) { }

  using ComputeVisitor::visit;
  virtual void visit(const X86ConvRelu&) { }
  virtual void visit(X86ConvRelu&) { }

  static bool classof(const ComputeVisitor* pOp);
};

} // namespace of onnc

#endif
