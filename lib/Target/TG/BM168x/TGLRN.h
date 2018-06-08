#ifndef ONNX_BM_TGLRN_H
#define ONNX_BM_TGLRN_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>

// m_MemOperands: input, output
namespace onnc {

class TGLRN : public ComputeOperator2
{
public:
  TGLRN(const ::onnx::Node &node);

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
};

} // namespace onnc

#endif
