#ifndef ONNX_BM_TGSOFTMAX
#define ONNX_BM_TGSOFTMAX

#include "onnx/common/ir.h"
#include "TGOperator.h"

class TGSoftmax : public TGOperator {
public:
  TGSoftmax(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
private:
  uint64_t m_inputAddr;
  uint64_t m_outputAddr;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

#endif
