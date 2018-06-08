#ifndef ONNX_BM1880_TGGEMM_H
#define ONNX_BM1880_TGGEMM_H

#include "ComputeOperand.h"
#include "bm188x_common_calibration.pb.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, weight, bias, output
class TGGemm : public ComputeOperand2
{
public:
  TGGemm(const ::onnx::Node &pNode,
         const tg::bm1880::LayerCalibrationParameter &pLayerCtable);
  void emit() const override;
  void print(OStream &pOS) const override;
  TGGemm *addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                         MemOperand *pWeight, MemOperand *pBias);

private:
  int m_inRowNum;
  int m_inColNum;
  int m_outColNum;
  int m_haveBias;
  bool m_weightTp;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
