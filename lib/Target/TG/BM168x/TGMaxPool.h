#ifndef ONNX_BM_TGMAXPOOL_H
#define ONNX_BM_TGMAXPOOL_H

#include "ComputeOperand.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: input, output
class TGMaxPool : public ComputeOperand2
{
public:
  TGMaxPool(const ::onnx::Node &node);

  void emit() const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_kH;
  int m_kW;
  int m_padH;
  int m_padW;
  int m_strideH;
  int m_strideW;
};

} // namespace onnc

#endif
