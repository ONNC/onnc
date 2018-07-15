#ifndef ONNX_BM1880_TGLRN_H
#define ONNX_BM1880_TGLRN_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGLRN : public BM188xComputeOperator
{
public:
  TGLRN(const ::onnx::Node &pNode);

  TGLRN *addMemOperands(MemOperand *pInput, MemOperand *pSquLut,
                        MemOperand *pPowerLut, MemOperand *pOutput);

  void emit() const override;
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

  float getAlpha() { return m_Alpha; }
  float getBeta() { return m_Beta; }
  float getBias() { return m_K; }
  int getLocalSize() { return m_LocalSize; }

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  float m_Alpha;
  float m_Beta;
  // bias
  float m_K;
  int m_LocalSize;
  int m_SumRightShiftWidth;
  int m_LrnRightShiftWidth;
  int m_ThresholdXQuantized[2];
};

} // namespace BM188X
} // namespace onnc

#endif
