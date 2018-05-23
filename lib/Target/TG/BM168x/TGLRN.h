#ifndef ONNX_BM_TGLRN_H
#define ONNX_BM_TGLRN_H

#include "Operator.h"
#include <onnx/common/ir.h>

namespace onnc {

class TGLRN : public Operator
{
public:
  TGLRN(const ::onnx::Node &node, MemTable &memTable);

  void emit() const override;

private:
  uint64_t m_inputAddr;
  uint64_t m_outputAddr;
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
