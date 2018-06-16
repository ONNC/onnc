#define DEBUG_TYPE "tg_maxpool"
#include "TGMaxPool.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGMaxPool::TGMaxPool(const ::onnx::Node &pNode,
                     const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
    : BM188xComputeOperator(pNode, std::string("MaxPool")), m_PadH(0),
      m_PadW(0), m_StrideH(1), m_StrideW(1), m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  m_KH = pNode.is(::onnx::Symbol("kernel_shape"))[0];
  m_KW = pNode.is(::onnx::Symbol("kernel_shape"))[1];

  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel padding on both ends
    m_PadH = i[0];
    m_PadW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }
}

TGMaxPool *TGMaxPool::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGMaxPool::print(OStream &pOS) const
{
  int rShiftWidth = m_LayerCtable.right_shift_width();
  const int *thresholdXQuantized = m_LayerCtable.threshold_x_quantized().data();
  pOS << *m_MemOperands[1] << " = MaxPool <N:" << m_N << ", C:" << m_C
      << ", H:" << m_H << ", W:" << m_W << ",  kH:" << m_KH << ", kW:" << m_KW
      << ", padH:" << m_PadH << ", padW:" << m_PadW << ", srideH:" << m_StrideH
      << ", strideW:" << m_StrideW << ", rShiftWidth:" << rShiftWidth
      << ", thresholdX:" << thresholdXQuantized[0] << "> (" << *m_MemOperands[0]
      << ")\n";
}

void TGMaxPool::emit() const
{
  DEBUG(print(dbgs()));

  int rShiftWidth = m_LayerCtable.right_shift_width();
  const int *thresholdXQuantized = m_LayerCtable.threshold_x_quantized().data();
  bmnet::bmnet_pooling_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX,
      m_MemOperands[0]->m_Addr, // ifmap_gaddr
      m_MemOperands[1]->m_Addr, // ofmap_gaddr
      GADDR_INVALID,            // index_gaddr
      GADDR_INVALID,            // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_KH, m_KW, m_PadH, m_PadW, m_StrideH, m_StrideW,
      0,                  // is_avg_pooling
      0.0f,               // avg_const
      0,                  // do_relu
      rShiftWidth,        // right_shift_width
      thresholdXQuantized // threshold_x_quantized
  );
}

void TGMaxPool::toASM(tg::bm1880::Insn *pI) const
{
  pI->set_name(getLayerName());
  pI->set_type(tg::bm1880::Insn::MaxPool);
  {
    auto *max_pool = pI->mutable_max_pool_param();
    {
      auto *input = max_pool->mutable_input();
      input->set_n(m_N);
      input->set_c(m_C);
      input->set_h(m_H);
      input->set_w(m_W);
      auto *m = m_MemOperands.at(0);
      input->set_addr(m->m_Addr);
      input->set_type(tg::bm1880::Operand::Int8);
      assert(m->m_Size == m->m_Count * 1);
      input->set_count(m->m_Count);
    }
    {
      auto *output = max_pool->mutable_output();
      auto *m = m_MemOperands.at(1);
      output->set_addr(m->m_Addr);
      output->set_type(tg::bm1880::Operand::Int8);
      assert(m->m_Size == m->m_Count * 1);
      output->set_count(m->m_Count);
    }
    {
      auto *k = max_pool->mutable_kernel();
      k->set_h(m_KH);
      k->set_w(m_KW);
    }
    {
      auto *p = max_pool->mutable_pad();
      p->set_h(m_PadH);
      p->set_w(m_PadW);
    }
    {
      auto *s = max_pool->mutable_stride();
      s->set_h(m_StrideH);
      s->set_w(m_StrideW);
    }
    {
      auto *cal = max_pool->mutable_ctable();
      cal->set_right_shift_width(m_LayerCtable.right_shift_width());
    }
  }
}

} // namespace BM188X
} // namespace onnc
