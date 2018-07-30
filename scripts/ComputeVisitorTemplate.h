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

/// ONNC defined operators
class Initializer;
class InputOperator;
class OutputOperator;

/// ONNX defined operators
${forward_declaration}

/** \class ComputeVisitor
 *  \brief ComptueVisitor provides interface to visit operators.
 */
class ComputeVisitor
{
public:
  typedef const void* VisitorTypeID;

public:
  ComputeVisitor() : m_VisitorID(nullptr) { }

  ComputeVisitor(char& pID) : m_VisitorID(&pID) { }

  bool hasVisitorID() const { return (nullptr != m_VisitorID); }

  VisitorTypeID getVisitorID() const { return m_VisitorID; }

  /// ONNC defined operators @{
  virtual void visit(const Initializer& pInitializer) { }
  virtual void visit(const InputOperator& pInputOperator) { }
  virtual void visit(const OutputOperator& pOutputOperator) { }

  /// @}
  /// ONNX defined operators @{
  ${const_visitor_member_functions}
  /// @}

  /// ONNC defined operators @{
  virtual void visit(Initializer& pInitializer) { }
  virtual void visit(InputOperator& pInputOperator) { }
  virtual void visit(OutputOperator& pOutputOperator) { }

  /// @}
  /// ONNX defined operators @{
  ${visitor_member_functions}
  /// @}

private:
  VisitorTypeID m_VisitorID;
};

} // namespace of onnc

#endif
