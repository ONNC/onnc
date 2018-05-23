#ifndef ONNX_BM_TGSOFTMAX
#define ONNX_BM_TGSOFTMAX

#include "Operator.h"
#include <onnx/common/ir.h>

namespace onnc {

class TGSoftmax : public Operator
{
public:
  TGSoftmax(const ::onnx::Node &node, MemTable &memTable);

  void emit() const override;

private:
  uint64_t m_inputAddr;
  uint64_t m_outputAddr;
  int m_N;
  int m_C;
  int m_H;
  int m_W;
};

} // namespace onnc

#endif
