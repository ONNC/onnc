#ifndef ONNX_BM_TGRELU_H
#define ONNX_BM_TGRELU_H

#include "onnx/common/ir.h"
#include "TGOperator.h"

class TGRelu : public TGOperator {
public:
  TGRelu(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
private:
  uint64_t m_inputAddr;
  uint64_t m_outputAddr;
  float m_negativeSlope;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};


#endif
