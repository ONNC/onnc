//===- Use.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_USE_H
#define ONNC_IR_COMPUTE_USE_H
#include <string>

namespace onnc {

class ComputeOperator;
class Value;

/** \class Use
 *  \brief Use represents an edge between its users and definition.
 *
 *  Unlike conventional compiler, in neural network, an operator may have
 *  multiple operands, thus, we need two fields - getUser() and getOperandNo()
 *  to locate one operand. An use represents certain input Value object of
 *  a ComputeOperator.
 */
class Use
{
public:
  /// create a use pointing to number @ref pIdx input value of operator @ref pOp
  /// If @ref pIdx is out of range of @ref pOperator's input value, than an fatal
  /// error occurs.
  Use(ComputeOperator& pOperator, unsigned int pIdx);

  ~Use() { }

  /// get the concert value.
  onnc::Value* get() { return m_pValue; }

  /// get the concert value.
  const onnc::Value* get() const { return m_pValue; }

  /// The user operator.
  ComputeOperator* getUser() { return m_pUser; }

  /// The user operator.
  const ComputeOperator* getUser() const { return m_pUser; }

  /// The index of input values of the user operator.
  unsigned int getOperandNo() const { return m_OperandNo; }

private:
  ComputeOperator* m_pUser;
  unsigned int m_OperandNo;
  onnc::Value *m_pValue;
};

} // namespace of onnc

#endif
