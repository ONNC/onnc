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

namespace onnc {

/** \class ComputeOperator
 */
class ComputeOperator : public DigraphNode<ComputeOperator, ComputeOperand>
{
public:
  ComputeOperator(const ::onnx::Node* pOperator = nullptr)
    : m_OpCode(0), m_pNode(pOperator) {
  }

  void setOpcode(unsigned int pOP) { m_OpCode = pOP; }

  unsigned int getOpCode() const { return m_OpCode; }

  unsigned int getNumOfOperands() const { return m_Operands.size(); }

  void addOperand(const ComputeOperand& pOperand) { m_Operands.push_back(pOperand); }

  void print(std::ostream& pOS) const;

private:
  typedef std::vector<ComputeOperand> OperandList;

private:
  unsigned int m_OpCode;
  OperandList m_Operands;
  const ::onnx::Node* m_pNode;
};

} // namespace of onnc

#endif
