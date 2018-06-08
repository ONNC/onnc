#ifndef ONNX_BM_TGRELU_H
#define ONNX_BM_TGRELU_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: input, output
class TGRelu : public ComputeOperator2
{
public:
  TGRelu(const ::onnx::Node &node);

  void emit() const override;

private:
  float m_negativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace onnc

#endif
