#ifndef ONNX_BM_TGConv_H
#define ONNX_BM_TGConv_H

#include <stdint.h>
class TGConv : public TGOperator {
public:
  TGConv(const onnx::Node &node, uint64_t offset);
  virtual void emit(void) const;
private:
  std::vector<uint64_t> m_weightOffset;
  uint32_t m_input_n, m_input_c, m_input_h, m_input_w;
  uint32_t m_output_c;
  uint32_t m_groups;
  uint16_t m_kh, m_kw;
  uint16_t m_dilation_h, m_dilation_w;
  uint8_t m_pad_h, m_pad_w;
  uint8_t m_stride_h, m_stride_w;
};

#endif
