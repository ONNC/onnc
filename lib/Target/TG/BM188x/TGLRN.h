#ifndef ONNX_BM1880_TGLRN_H
#define ONNX_BM1880_TGLRN_H

#include "ComputeOperand.h"
#include "common_calibration.pb.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGLRN : public ComputeOperand2
{
public:
  TGLRN(const ::onnx::Node &node,
        const LayerCalibrationParameter &pLayerCtable);

  void emit() const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  float m_alpha;
  float m_beta;
  // bias
  float m_k;
  int m_localSize;
  LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
