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
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/Bits/DigraphNode.h>
#include <onnc/IR/ComputeDefine.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeAttrOperand.h>
#include <onnc/IR/ComputeMemOperand.h>
#include <onnc/Support/IOStream.h>
#include <vector>

namespace onnc {

class ComputeVisitor;

/** \class ComputeOperator
 *
 *  ComputeOperator connects ONNX and machine ABI.
 */
class ComputeOperator : public ComputeDefine,
                        public DigraphNode<ComputeOperator, ComputeOperand>
{
public:
  using GraphOperator = ::onnx::Node;

  typedef int64_t Opcode;

public:
  ComputeOperator(StringRef pName)
    : ComputeDefine(pName),
      m_OpCode(0), m_Inputs(), m_Outputs(), m_GraphOperators() {
  }

  void setOpcode(Opcode pOP) { m_OpCode = pOP; }

  Opcode getOpCode() const { return m_OpCode; }

  /// Connect one compute operator to a graph operator @ref pGraphOperator
  /// One compute operator may connect to multiple graph operators.
  void connect(const GraphOperator& pGraphOperator);

  unsigned int getNumOfGraphOperators() const { return m_GraphOperators.size(); }

  unsigned int getNumOfInputs() const { return m_Inputs.size(); }

  unsigned int getNumOfOutputs() const { return m_Outputs.size(); }

  template<typename OpndType>
  void addInputs(OpndType& pOperand) { m_Inputs.push_back(&pOperand); }

  template<typename OpndType>
  void addOutputs(OpndType& pOperand) { m_Outputs.push_back(&pOperand); }

  /// redirect the printing to stderr
  void dump() const { return print(errs()); }

  /// entrance ramp for visitor
  virtual void accept(ComputeVisitor& pVisitor) = 0;

private:
  typedef std::vector<const GraphOperator*>  GraphOperatorList;
  typedef std::vector<ComputeOperand*> OperandList;

private:
  Opcode m_OpCode;
  OperandList m_Inputs;
  OperandList m_Outputs;

  /// one compute operator may point to multiple graph operator.
  GraphOperatorList m_GraphOperators;
};

} // namespace of onnc

#endif
