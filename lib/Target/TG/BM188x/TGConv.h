#ifndef ONNX_BM1880_TGCONV_H
#define ONNX_BM1880_TGCONV_H

#include "BM188xComputeOperator.h"
#include "TGBackend.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_emOperands: ifmap, weight, ofmap, bias
class TGConv : public BM188xComputeOperator
{
public:
  TGConv(const ::onnx::Node &pNode);

  void emit() const override;
  void prepareWeight(std::vector<int8_t> &pWeight);
  void print(OStream &pOS) const override;
  TGConv *addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                         MemOperand *pWeight, MemOperand *pBias);
  void toASM(tg::bm1880::Inst *pI) const override;
  void
  update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable) override;

private:
  int m_InN, m_InC, m_InH, m_InW;
  int m_Groups;
  int m_OutC;
  uint16_t m_KH, m_KW;
  uint16_t m_DilationH, m_DilationW;
  uint8_t m_PadH, m_PadW;
  uint8_t m_StrideH, m_StrideW;
  int m_DoBias;
  int m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif
