#ifndef ONNX_BM1880_TGPRELU_H
#define ONNX_BM1880_TGPRELU_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

class TGPRelu : public BM188xComputeOperator
{
public:
  TGPRelu(const ::onnx::Node &pNode);

  void emit() const override;
  void print(OStream &pOS) const override;
  // m_MemOperands: input, slope, output
  TGPRelu *addMemOperands(MemOperand *pInput, MemOperand *pSlope,
                          MemOperand *pOutput);
  void toASM(tg::bm1880::Inst *pI) const override;
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  float m_Slope;
  int m_ChannelShared;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_GTRShiftWidth;
  int m_LERShiftWidth;
  int m_GTScale;
};

} // namespace BM188X
} // namespace onnc

#endif
