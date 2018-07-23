#ifndef ONNX_BM1880_TGSCALE_H
#define ONNX_BM1880_TGSCALE_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGScale : public BM188xComputeOperator
{
public:
  TGScale(const ::onnx::Node &pNode);

  TGScale *addMemOperands(MemOperand *pInput, MemOperand *pScale,
                          MemOperand *pBias, MemOperand *pOutput);
  void emit() const override;
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_MScaleDim;
  int m_MInnerDim;
  int m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif
