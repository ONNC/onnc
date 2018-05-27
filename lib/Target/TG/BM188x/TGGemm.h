#ifndef ONNX_BM1880_TGGEMM_H
#define ONNX_BM1880_TGGEMM_H

#include "ComputeOperand.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

// m_MemOperands: input, weight, bias, output
class TGGemm : public ComputeOperand2
{
public:
  TGGemm(const ::onnx::Node &pNode);
  void emit() const override;

private:
  int m_inRowNum;
  int m_inColNum;
  int m_outColNum;
  int m_haveBias;
  int m_usingRelu;
  bool m_weightTp;
};

} // namespace BM188X
} // namespace onnc

#endif
