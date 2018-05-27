#ifndef ONNX_BM1880_TGSOFTMAX
#define ONNX_BM1880_TGSOFTMAX

#include "ComputeOperand.h"
#include <onnx/common/ir.h>

namespace onnc {
namespace BM188X {

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

} // namespace BM188X
} // namespace onnc

#endif
