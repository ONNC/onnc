//===- X86ComputeVisitor.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86ComputeVisitor.h"

using namespace onnc;

class X86ConvRelu;

char X86ComputeVisitor::ID = 0;

bool X86ComputeVisitor::classof(const ComputeVisitor* pOp) {
  if (nullptr == pOp)
    return false;
  return (pOp->getVisitorID() == &ID);
}
