#ifndef ONNX_BM1880_TGMAXPOOL_H
#define ONNX_BM1880_TGMAXPOOL_H

#include "ComputeOperand.h"
#include "common_calibration.pb.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGMaxPool : public ComputeOperand2
{
public:
  TGMaxPool(const ::onnx::Node &node,
            const LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;

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
  LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
