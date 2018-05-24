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

namespace onnc {

/** \class ComputeOperand
 * 
 */
class ComputeOperand
{
public:
  enum Type : unsigned char {
    kInvalid,
    kRegister,
    kImmediate,
  };

public:
  ComputeOperand() : m_Type(kInvalid) { }

  bool isValid() const { return kInvalid != m_Type; }

  bool isReg() const { return kRegister == m_Type; }

  bool isImm() const { return kImmediate == m_Type; }

  void print(std::ostream& pOS) const;

private:

  Type m_Type;

  union {
    unsigned int m_RegVal;
    int64_t m_ImmVal;
  };
};

class ComputeMemOperand
{
public:
  unsigned getBankID() const;

private:
  unsigned m_BankID;
};

class ComputeAttrOperand
{
};

/** \class ComputeOperator
 */
class ComputeOperator
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
