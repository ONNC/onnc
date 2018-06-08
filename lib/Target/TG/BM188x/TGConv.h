#ifndef ONNX_BM1880_TGCONV_H
#define ONNX_BM1880_TGCONV_H

#include "ComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: ifmap, weight, ofmap, bias
class TGConv : public ComputeOperator2
{
public:
  TGConv(const ::onnx::Node &pNode,
         const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGConv *addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                         MemOperand *pWeight, MemOperand *pBias);

private:
  int m_inN, m_inC, m_inH, m_inW;
  int m_groups;
  int m_outC;
  uint16_t m_kH, m_kW;
  uint16_t m_dilationH, m_dilationW;
  uint8_t m_padH, m_padW;
  uint8_t m_strideH, m_strideW;
  int m_doBias;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
