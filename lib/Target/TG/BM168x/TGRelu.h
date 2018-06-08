#ifndef ONNX_BM_TGRELU_H
#define ONNX_BM_TGRELU_H

#include "Operator.h"
#include <onnx/common/ir.h>

namespace onnc {

class TGRelu : public Operator
{
public:
  TGRelu(const ::onnx::Node &node);

  void emit() const override;

private:
  uint64_t m_inputAddr;
  uint64_t m_outputAddr;
  float m_negativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace onnc

#endif
