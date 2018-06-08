#ifndef ONNX_BM1880_TGLRN_H
#define ONNX_BM1880_TGLRN_H

#include "ComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGLRN : public ComputeOperator2
{
public:
  TGLRN(const ::onnx::Node &node,
        const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

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
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
