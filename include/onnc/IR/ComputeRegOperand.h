//===- ComputeRegOperand.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_REGISTER_OPERAND_H
#define ONNC_IR_COMPUTE_REGISTER_OPERAND_H
#include <ostream>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/Config/ONNX.h>

namespace onnc {

/** \class ComputeRegOperand
 * 
 */
class ComputeRegOperand : public ComputeOperand
{
public:
  enum Type : unsigned char {
    kInvalid,
    kRegister,
    kImmediate,
  };

public:
  ComputeRegOperand();

  virtual ~ComputeRegOperand();

  bool isValid() const { return kInvalid != m_Type; }

  bool isReg() const { return kRegister == m_Type; }

  bool isImm() const { return kImmediate == m_Type; }

  void print(std::ostream& pOS) const;

  static bool classof(const ComputeOperand* pOpnd) {
    return (ComputeOperand::kRegOperand == pOpnd->kind());
  }

private:

  Type m_Type;

  union {
    unsigned int m_RegVal;
    int64_t m_ImmVal;
  };
};


} // namespace of onnc

#endif
