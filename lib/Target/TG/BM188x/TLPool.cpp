#include "TLPool.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tl_pool"

namespace onnc {
namespace BM188X {

TLPool::TLPool(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("TLPool"))
{
  // Default value
  m_PadHTop = m_PadHBot = m_PadWLeft = m_PadWRight = 0;
  m_StrideH = m_StrideW = 1;
  m_RShiftWidth = 0;
  m_ThresholdXQuantized = 0;

  // ONNC extension attribute
  assert(pNode.hasAttribute(::onnx::Symbol("op_name")));
  m_SplitName = pNode.s(::onnx::Symbol("op_name"));

  assert(pNode.hasAttribute(::onnx::Symbol("input_dim")));
  assert(pNode.hasAttribute(::onnx::Symbol("output_dim")));

  auto &inDim = pNode.is(::onnx::Symbol("input_dim"));
  m_InN = inDim[0];
  m_InC = inDim[1];
  m_InH = inDim[2];
  m_InW = inDim[3];

  auto &outDim = pNode.is(::onnx::Symbol("output_dim"));
  m_OutN = outDim[0];
  m_OutC = outDim[1];
  m_OutH = outDim[2];
  m_OutW = outDim[3];

  assert(pNode.hasAttribute(::onnx::Symbol("ifmap_laddr")));
  assert(pNode.hasAttribute(::onnx::Symbol("ofmap_laddr")));

  m_IFmapAddr = pNode.i(::onnx::Symbol("ifmap_laddr"));
  m_OFmapAddr = pNode.i(::onnx::Symbol("ofmap_laddr"));

  assert(pNode.hasAttribute(::onnx::Symbol("is_avg_pooling")));
  m_IsAvgPooling = pNode.i(::onnx::Symbol("is_avg_pooling"));
  // End extension

  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    assert(i.size() == 2);
    m_KH = i[0];
    m_KW = i[1];
  }

  // [upPad, leftPad, downPad, rightPad]
  if (pNode.hasAttribute(::onnx::Symbol("slice_pads"))) {
    auto &i = pNode.is(::onnx::Symbol("slice_pads"));
    m_PadHTop = i[0];
    m_PadWLeft = i[1];
    m_PadHBot = i[2];
    m_PadWRight = i[3];
  }

  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }
}

TLPool *TLPool::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TLPool::emit() const
{
  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  bmnet::bmnet_asm::bmnet_tl_pooling_forward_bmkernel(
      m_IFmapAddr, // ifmap
      m_OFmapAddr, // ofmap
      m_InN, m_InC, m_InH, m_InW, m_OutN, m_OutC, m_OutH, m_OutW, m_KH, m_KW,
      m_StrideH, m_StrideW,                          // stride
      m_PadHTop, m_PadHBot, m_PadWLeft, m_PadWRight, // padding
      m_IsAvgPooling,                                // is_avg_pooling
      m_RShiftWidth, m_ThresholdXQuantized);
}

void TLPool::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ThresholdXQuantized = *pLayerCtable->threshold_x_quantized().data();
}

} // namespace BM188X
} // namespace onnc
