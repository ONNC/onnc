#ifndef ONNX_BM_TGSOFTMAX
#define ONNX_BM_TGSOFTMAX

#include "ComputeOperand.h"
#include <onnx/common/ir.h>

namespace onnc {

// m_MemOperands: input, output
class TGSoftmax : public ComputeOperand2
{
public:
  TGSoftmax(const ::onnx::Node &node);

  void emit() const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace onnc

#endif
