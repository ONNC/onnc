#ifndef ONNX_BM_TGGEMM_H
#define ONNX_BM_TGGEMM_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>
namespace onnc {

//  m_EmOperands: input, weight, bias, output
class TGGemm : public ComputeOperator2
{
public:
  TGGemm(const ::onnx::Node &pNode);
  void emit() const override;

private:
  int m_InRowNum;
  int m_InColNum;
  int m_OutColNum;
  int m_HaveBias;
  int m_UsingRelu;
  bool m_WeightTp;
};

} // namespace onnc

#endif
