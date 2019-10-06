//===- ComputeVisitor.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_VISITOR_H
#define ONNC_IR_COMPUTE_VISITOR_H

#include <cstdint>

namespace onnc {

/// ONNC defined operators
class Initializer;
class InputOperator;
class OutputOperator;

/// ONNX defined operators
${forward_declaration}

/** \class ComputeVisitor
 *  \brief ComputeVisitor provides interface to visit operators.
 */
class ComputeVisitor
{
public:
  using VisitorTypeID = std::intptr_t;

public:
  ComputeVisitor() = default;

  virtual VisitorTypeID getVisitorID() const = 0;

  /// ONNC defined operators @{
  virtual void visit(const Initializer&) { }
  virtual void visit(const InputOperator&) { }
  virtual void visit(const OutputOperator&) { }

  /// @}
  /// ONNX defined operators @{
  ${const_visitor_member_functions}
  /// @}

  /// ONNC defined operators @{
  virtual void visit(Initializer& pInitializer) { visit(const_cast<const Initializer&>(pInitializer)); }
  virtual void visit(InputOperator& pInputOperator) { visit(const_cast<const InputOperator&>(pInputOperator)); }
  virtual void visit(OutputOperator& pOutputOperator) { visit(const_cast<const OutputOperator&>(pOutputOperator)); }

  /// @}
  /// ONNX defined operators @{
  ${visitor_member_functions}
  /// @}
};

} // namespace of onnc

#endif
