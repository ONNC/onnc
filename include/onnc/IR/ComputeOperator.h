//===- ComputeOperator.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_H
#define ONNC_IR_COMPUTE_OPERATOR_H
#include <ostream>
#include <onnx/common/ir.h>
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeAttrOperand.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <vector>

namespace onnc {

/** \class ComputeOperator
 */
class ComputeOperator : public DigraphNode<ComputeOperator, ComputeOperand>
{
public:
  using GraphOperator = ::onnx::Node;

public:
  ComputeOperator()
    : m_OpCode(0), m_Operands(), m_GraphOperators() {
  }

  void setOpcode(unsigned int pOP) { m_OpCode = pOP; }

  unsigned int getOpCode() const { return m_OpCode; }

  /// Connect one compute operator to a graph operator @ref pGraphOperator
  /// One compute operator may connect to multiple graph operators.
  void connect(const GraphOperator& pGraphOperator);

  unsigned int getNumOfGraphOperators() const { return m_GraphOperators.size(); }

  unsigned int getNumOfOperands() const { return m_Operands.size(); }

  void addOperand(ComputeOperand& pOperand) { m_Operands.push_back(&pOperand); }

  void print(std::ostream& pOS) const;

private:
  typedef std::vector<const GraphOperator*>  GraphOperatorList;

  // TODO: introduce different types of operand here.
  typedef std::vector<ComputeOperand*> OperandList;

private:
  unsigned int m_OpCode;
  OperandList m_Operands;
  GraphOperatorList m_GraphOperators;
};

} // namespace of onnc

#endif
