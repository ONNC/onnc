#ifndef ONNX_BM1880_TGMAXPOOL_H
#define ONNX_BM1880_TGMAXPOOL_H

#include "ComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGMaxPool : public ComputeOperator2
{
public:
  TGMaxPool(const ::onnx::Node &pNode,
            const tg::bm1880::LayerCalibrationParameter &pLayerCtable);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGMaxPool *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void toASM(tg::bm1880::Insn *pI) const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  int m_KH;
  int m_KW;
  int m_PadH;
  int m_PadW;
  int m_StrideH;
  int m_StrideW;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
