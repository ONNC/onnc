#ifndef ONNX_BM1880_TGRELU_H
#define ONNX_BM1880_TGRELU_H

#include "ComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGRelu : public ComputeOperator2
{
public:
  TGRelu(const ::onnx::Node &node,
         const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGRelu *addMemOperands(MemOperand *pInput, MemOperand *pOutput);

private:
  float m_negativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
