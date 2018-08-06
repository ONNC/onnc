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
#include <onnc/IR/ComputeOperand.h>
#include <onnc/Config/ONNX.h>

namespace onnc {

/** \class ComputeMemOperand
 * 
 */
class ComputeMemOperand : public ComputeOperand
{
public:
  ComputeMemOperand();

  ComputeMemOperand(uint32_t pStart, uint32_t pLength);

  ComputeMemOperand(onnc::Value& pValue, Residence pResidence);

  virtual ~ComputeMemOperand();

  void setStart(uint32_t pStart) { m_Start = pStart; }

  uint32_t start() const { return m_Start; }

  void setLength(uint32_t pLen) { m_Length = pLen; }

  uint32_t length() const { return m_Length; }

  static bool classof(const ComputeOperand* pOpnd) {
    return (ComputeOperand::kMemOperand == pOpnd->kind());
  }

private:
  uint32_t m_Start;
  uint32_t m_Length;
};

} // namespace of onnc

#endif
