#ifndef ONNX_BM_TGGEMM_H
#define ONNX_BM_TGGEMM_H

#include "Operator.h"
#include <onnx/common/ir.h>
namespace onnc {

class TGGemm : public Operator
{
public:
  TGGemm(const ::onnx::Node &pNode);
  void dumpOnnxGemm(const ::onnx::Node &pNode);
  void emit() const override;

private:
  uint64_t m_inputAddr;
  uint64_t m_weightAddr;
  uint64_t m_biasAddr;
  uint64_t m_outputAddr;
  int m_inRowNum;
  int m_inColNum;
  int m_outColNum;
  int m_haveBias;
  int m_usingRelu;
  bool m_weightTp;
};

} // namespace onnc

#endif
