#ifndef ONNX_BM1880_TGMAXPOOL_H
#define ONNX_BM1880_TGMAXPOOL_H

#include "ComputeOperator.h"
#include "bm188x_common_calibration.pb.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGMaxPool : public ComputeOperator2
{
public:
  TGMaxPool(const ::onnx::Node &node,
            const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGMaxPool *addMemOperands(MemOperand *pInput, MemOperand *pOutput);

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
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
