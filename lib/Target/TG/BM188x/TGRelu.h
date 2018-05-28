#ifndef ONNX_BM1880_TGRELU_H
#define ONNX_BM1880_TGRELU_H

#include "ComputeOperand.h"
#include "common_calibration.pb.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGRelu : public ComputeOperand2
{
public:
  TGRelu(const ::onnx::Node &node,
         const LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;

private:
  float m_negativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
