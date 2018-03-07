#ifndef ONNX_BM_TGConv_H
#define ONNX_BM_TGConv_H

class TGConv : public TGOperator {
public:
  TGConv(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
private:
  std::vector<uint64_t> m_weightOffset;
};

#endif
