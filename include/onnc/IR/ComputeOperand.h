//===- ComputeOperand.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERAND_H
#define ONNC_IR_COMPUTE_OPERAND_H
#include <onnc/ADT/Bits/DigraphArc.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Compute/Value.h>
#include <ostream>

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
  enum Kind {
    kRegOperand,
    kMemOperand
  };

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
  ComputeOperand(Kind pKind);

  /// Points to @ref pValue
  ComputeOperand(Kind pKind, onnc::Value& pValue, Residence pResidence);

  virtual ~ComputeOperand();

  bool hasValue() const { return (nullptr != m_pValue); }

  onnc::Value* getValue() { return m_pValue; }

  const onnc::Value* getValue() const { return m_pValue; }

  void setResidence(Residence pResidence) { m_Residence = pResidence; }

  Residence residence() const { return m_Residence; }

  bool isInput() const { return (kInputResidence == m_Residence); }

  bool isWeight() const { return (kWeightResidence == m_Residence); }

  bool isOutput() const { return (kOutputResidence == m_Residence); }

  bool isInternal() const { return (kInternalResidence == m_Residence); }

  bool isUnknown() const { return (kUnknownResidence == m_Residence); }

  Kind kind() const { return m_Kind; }

protected:
  Kind m_Kind;
  onnc::Value* m_pValue;
  Residence m_Residence;
};


} // namespace of onnc

#endif
