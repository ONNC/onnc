#ifndef ONNX_BM1880_TGGlobalAVERAGEPOOL_H
#define ONNX_BM1880_TGGlobalAVERAGEPOOL_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGGlobalAveragePool : public BM188xComputeOperator
{
public:
  TGGlobalAveragePool(const ::onnx::Node &pNode);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGGlobalAveragePool *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_KH;
  int m_KW;
  int m_PadH;
  int m_PadW;
  int m_StrideH;
  int m_StrideW;
  int m_EnableRelu;
  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
