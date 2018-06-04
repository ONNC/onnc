#define DEBUG_TYPE "tg_maxpool"
#include "TGMaxPool.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGMaxPool::TGMaxPool(const ::onnx::Node &pNode,
                     const LayerCalibrationParameter &pLayerCtable)
    : ComputeOperand2(pNode, "MaxPool"), m_padH(0), m_padW(0), m_strideH(1),
      m_strideW(1), m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  m_kH = pNode.is(::onnx::Symbol("kernel_shape"))[0];
  m_kW = pNode.is(::onnx::Symbol("kernel_shape"))[1];

  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel padding on both ends
    m_padH = i[0];
    m_padW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
}

void TGMaxPool::print(OStream &pOS) const
{

  pOS << *m_MemOperands[1] << " = MaxPool <N:" << m_N << ", C:" << m_C
      << ", H:" << m_H << ", W:" << m_W << ",  kH:" << m_kH << ", kW:" << m_kW
      << ", padH:" << m_padH << ", padW:" << m_padW << ", srideH:" << m_strideH
      << ", strideW:" << m_strideW << "> (" << *m_MemOperands[0] << ")\n";
}

void TGMaxPool::emit() const
{
  DEBUG(print(dbgs()));

  int rShiftWidth = m_LayerCtable.right_shift_width();
  const int *thresholdXQuantized = m_LayerCtable.threshold_x_quantized().data();
  bmnet::bmnet_pooling_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx,
      m_MemOperands[0]->addr, // ifmap_gaddr
      m_MemOperands[1]->addr, // ofmap_gaddr
      GADDR_INVALID,          // index_gaddr
      GADDR_INVALID,          // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_kH, m_kW, m_padH, m_padW, m_strideH, m_strideW,
      0,                  // is_avg_pooling
      0.0f,               // avg_const
      0,                  // do_relu
      rShiftWidth,        // right_shift_width
      thresholdXQuantized // threshold_x_quantized
  );
}

} // namespace BM188X
} // namespace onnc
