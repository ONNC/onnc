#ifndef ONNX_BM_TGCONV_H
#define ONNX_BM_TGCONV_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: ifmap, weight, ofmap, bias
class TGConv : public ComputeOperator2
{
public:
  TGConv(const ::onnx::Node &node);

  void emit() const override;

private:
  int m_inN, m_inC, m_inH, m_inW;
  int m_groups;
  int m_outC;
  uint16_t m_kH, m_kW;
  uint16_t m_dilationH, m_dilationW;
  uint8_t m_padH, m_padW;
  uint8_t m_strideH, m_strideW;
  int m_doBias;
};

} // namespace onnc

#endif
