//===- ComputeOperand.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERAND_H
#define ONNC_IR_COMPUTE_OPERAND_H
#include <ostream>
#include <onnx/common/ir.h>
#include <onnc/ADT/Bits/DigraphArc.h>
#include <onnc/IR/Compute/Value.h>

namespace onnc {

class ComputeOperator;

/** \class ComputeOperand
 * 
 */
class ComputeOperand : public DigraphArc<ComputeOperator, ComputeOperand>
{
public:
  ComputeOperand();

  ComputeOperand(onnc::Value& pValue);

  bool hasValue() const { return (nullptr != m_pValue); }

  onnc::Value* getValue() { return m_pValue; }

  const onnc::Value* getValue() const { return m_pValue; }

protected:
  onnc::Value* m_pValue;
};


} // namespace of onnc

#endif
