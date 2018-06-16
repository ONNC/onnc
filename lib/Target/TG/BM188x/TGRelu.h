#ifndef ONNX_BM1880_TGRELU_H
#define ONNX_BM1880_TGRELU_H

#include "BM188xComputeOperator.h"
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, output
class TGRelu : public BM188xComputeOperator
{
public:
  TGRelu(const ::onnx::Node &pNode);

  void emit() const override;
  void print(OStream &pOS) const override;
  TGRelu *addMemOperands(MemOperand *pInput, MemOperand *pOutput);
  void toASM(tg::bm1880::Insn *pI) const override;

private:
  float m_NegativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace BM188X
} // namespace onnc

#endif
