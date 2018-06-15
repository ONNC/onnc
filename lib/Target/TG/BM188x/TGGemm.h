#ifndef ONNX_BM1880_TGGEMM_H
#define ONNX_BM1880_TGGEMM_H

#include "ComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_emOperands: input, weight, bias, output
class TGGemm : public ComputeOperator2
{
public:
  enum ActivationMethod { RELU = 0, SIGMOID, TANH, ELU, PRELU };
  TGGemm(const ::onnx::Node &pNode,
         const tg::bm1880::LayerCalibrationParameter &pLayerCtable);
  void emit() const override;
  void print(OStream &pOS) const override;
  TGGemm *addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                         MemOperand *pWeight, MemOperand *pBias);
  void toASM(tg::bm1880::Insn *pI) const override;

private:
  int m_InRowNum;
  int m_InColNum;
  int m_OutColNum;
  int m_HaveBias;
  bool m_WeightTp;
  bool m_EnableRelu;
  tg::bm1880::LayerCalibrationParameter m_LayerCtable;
};

} // namespace BM188X
} // namespace onnc

#endif
