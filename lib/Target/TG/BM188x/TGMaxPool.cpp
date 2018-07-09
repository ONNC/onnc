#define DEBUG_TYPE "tg_maxpool"
#include "TGMaxPool.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGMaxPool::TGMaxPool(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("MaxPool")), m_PadT(0),
      m_PadB(0), m_PadL(0), m_PadR(0), m_StrideH(1), m_StrideW(1),
      m_RShiftWidth(0), m_ThresholdXQuantized(0)
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
    m_PadT = i[0];
    m_PadL = i[1];
    m_PadB = i[2];
    m_PadR = i[3];
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
  pOS << *m_MemOperands[1] << " = MaxPool <N:" << m_N << ", C:" << m_C
      << ", H:" << m_H << ", W:" << m_W << ",  kH:" << m_KH << ", kW:" << m_KW
      << ", padT:" << m_PadT << ", padB:" << m_PadB << ", padL:" << m_PadL
      << ", padR:" << m_PadR << ", srideH:" << m_StrideH
      << ", strideW:" << m_StrideW << ", rShiftWidth:" << m_RShiftWidth
      << ", thresholdX:" << m_ThresholdXQuantized << "> (" << *m_MemOperands[0]
      << ")\n";
}

void TGMaxPool::emit() const
{
  DEBUG(print(dbgs()));

  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX,
      m_MemOperands[0]->m_Addr, // ifmap_gaddr
      m_MemOperands[1]->m_Addr, // ofmap_gaddr
      GADDR_INVALID,            // index_gaddr
      GADDR_INVALID,            // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_KH, m_KW, m_PadT, m_PadB, m_PadL, m_PadR, m_StrideH,
      m_StrideW,
      0,                     // is_avg_pooling
      0.0f,                  // avg_const
      0,                     // do_relu
      m_RShiftWidth,         // right_shift_width
      &m_ThresholdXQuantized // threshold_x_quantized
  );
}

void TGMaxPool::update(
    const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ThresholdXQuantized = *pLayerCtable->threshold_x_quantized().data();
}

} // namespace BM188X
} // namespace onnc
