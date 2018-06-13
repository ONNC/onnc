//===- ComputeVisitor.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VISITOR_H
#define ONNC_IR_COMPUTE_VISITOR_H

namespace onnc {

class Conv;

/** \class ComputeVisitor
 */
class ComputeVisitor
{
public:
  virtual void visit(Conv& pConv) = 0;
};

} // namespace of onnc

#endif
