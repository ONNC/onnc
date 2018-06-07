//===- ComputeMemOperand.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_MEMORY_OPERAND_H
#define ONNC_IR_COMPUTE_MEMORY_OPERAND_H
#include <ostream>
#include <onnx/common/ir.h>
#include <onnc/IR/ComputeOperand.h>

namespace onnc {

/** \class ComputeMemOperand
 * 
 */
class ComputeMemOperand : public ComputeOperand
{
public:
  unsigned getBankID() const;

private:
  unsigned m_BankID;
};

} // namespace of onnc

#endif
