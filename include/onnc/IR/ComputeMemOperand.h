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
  ComputeMemOperand()
    : m_Start(0), m_Length(0) {
  }

  ComputeMemOperand(uint32_t pStart, uint32_t pLength)
    : m_Start(pStart), m_Length(pLength) {
  }

  void setStart(uint32_t pStart) { m_Start = pStart; }

  uint32_t start() const { return m_Start; }

  void setLength(uint32_t pLen) { m_Length = pLen; }

  uint32_t length() const { return m_Length; }

private:
  uint32_t m_Start;
  uint32_t m_Length;
};

} // namespace of onnc

#endif
