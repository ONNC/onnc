#define DEBUG_TYPE "tg_globalaveragepool"
#include "TGGlobalAveragePool.h"
#include "BM188xCodeEmitter.h"
#include <bmnet/targets/plat-bm188x/bmkernel/bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGGlobalAveragePool::TGGlobalAveragePool(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("AveragePool")), m_EnableRelu(0),
      m_RShiftWidth(0), m_ThresholdXQuantized(0)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  if (pNode.hasAttribute(::onnx::Symbol("enable_relu"))) {
    m_EnableRelu = pNode.i(::onnx::Symbol("enable_relu"));
  }
}

TGGlobalAveragePool *TGGlobalAveragePool::addMemOperands(MemOperand *pInput,
                                                         MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGGlobalAveragePool::print(OStream &pOS) const
{
  pOS << *m_MemOperands[1] << " = GlobalAveragePool <N:" << m_N << ", C:" << m_C
      << ", H:" << m_H << ", W:" << m_W << ", rShiftWidth:" << m_RShiftWidth
      << ", thresholdX:" << m_ThresholdXQuantized << "> (" << *m_MemOperands[0]
      << ")\n";
}

void TGGlobalAveragePool::emit() const
{
  DEBUG(print(dbgs()));

  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX,
      m_MemOperands[0]->m_Addr, // ifmap_gaddr
      m_MemOperands[1]->m_Addr, // ofmap_gaddr
      GADDR_INVALID,            // index_gaddr
      GADDR_INVALID,            // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_H, m_W, 0, 0, 0, 0, 1, 1,
      1,                     // is_avg_pooling
      0.0f,                  // avg_const
      m_EnableRelu,          // do_relu
      m_RShiftWidth,         // right_shift_width
      &m_ThresholdXQuantized // threshold_x_quantized
  );
}

void TGGlobalAveragePool::update(
    const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ThresholdXQuantized = *pLayerCtable->threshold_x_quantized().data();
}

} // namespace BM188X
} // namespace onnc
