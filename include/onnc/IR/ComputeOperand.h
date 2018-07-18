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
 *  ComputeOperand provides base class for the description of scalar and memory
 *  reference for a backend.
 */
class ComputeOperand : public DigraphArc<ComputeOperator, ComputeOperand>
{
public:
  enum Residence {
    kInputResidence,    //< The operand refers to an input
    kWeightResidence,   //< The operand refers to a weight
    kOutputResidence,   //< The operand refers to an output
    kInternalResidence, //< The operand refers to an internal value
    kUnknownResidence
  };

public:
  /// Create a default ComputeOperand.
  /// Points to nowhere; internal memory resident
  ComputeOperand();

  /// Points to @ref pValue
  ComputeOperand(onnc::Value& pValue, Residence pResidence);

  virtual ~ComputeOperand();

  bool hasValue() const { return (nullptr != m_pValue); }

  onnc::Value* getValue() { return m_pValue; }

  const onnc::Value* getValue() const { return m_pValue; }

  void setResidence(Residence pResidence) { m_Residence = pResidence; }

  Residence residence() const { return m_Residence; }

protected:
  onnc::Value* m_pValue;
  Residence m_Residence;
};


} // namespace of onnc

#endif
