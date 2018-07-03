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

${forward_declaration}

/** \class ComputeVisitor
 *  \brief ComptueVisitor provides interface to visit operators.
 */
class ComputeVisitor
{
public:
  ${visitor_member_functions}
};

} // namespace of onnc

#endif
