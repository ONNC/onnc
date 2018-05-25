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


} // namespace of onnc

#endif
