#ifndef ONNX_BM1880_TGLRN_H
#define ONNX_BM1880_TGLRN_H

#include "ComputeOperand.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: input, output
class TGLRN : public ComputeOperand2
{
public:
  TGLRN(const ::onnx::Node &node);

  void emit() const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  float m_alpha;
  float m_beta;
  // bias
  float m_k;
  int m_localSize;
};

} // namespace onnc

#endif
