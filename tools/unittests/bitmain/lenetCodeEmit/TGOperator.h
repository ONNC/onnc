#include "onnx/common/ir.h"

class TGOperator {
public:
  TGOperator(const onnx::Node &node);
  virtual ~TGOperator() {};
  uint64_t getTotalSize(void) { return m_totalSize; }
  virtual void emit() const = 0;
private:
  uint64_t m_totalSize;
};


class TGConv : public TGOperator {
public:
  TGConv(const onnx::Node &node, uint64_t offset) : TGOperator(node) {}
  virtual void emit() const;
};
