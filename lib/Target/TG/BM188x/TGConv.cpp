#include "TGConv.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"

namespace pm = onnc::PatternMatch;

namespace onnc {
namespace BM188X {

// TGConv
TGConv::TGConv(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Conv")), m_Groups(1),
      m_DilationH(1), m_DilationW(1), m_PadH(0), m_PadW(0), m_StrideH(1),
      m_StrideW(1), m_DoBias(0), m_DoRelu(0), m_DoScale(0), m_DoScaleBias(0),
      m_RShiftWidth(0), m_ScaleRShiftWidth(0), m_ConvOutputThreshold(0)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_InN = inDim[0].dim;
  m_InC = inDim[1].dim;
  m_InH = inDim[2].dim;
  m_InW = inDim[3].dim;
  const std::vector< ::onnx::Dimension> weightDim = pNode.inputs()[1]->sizes();
  m_OutC = weightDim[0].dim;
  if (pNode.hasAttribute(::onnx::Symbol("group"))) {
    m_Groups = pNode.i(::onnx::Symbol("group"));
  }
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    m_KH = i[0];
    m_KW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("dilations"))) {
    auto &i = pNode.is(::onnx::Symbol("dilations"));
    m_DilationH = i[0];
    m_DilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_PadH = i[0];
    m_PadW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }

  if (pm::match(m_pNode, pm::mTrueAttr("do_scale"))) {
    m_DoScale = 1;
  }

  if (pm::match(m_pNode, pm::mTrueAttr("do_scale_bias"))) {
    m_DoScaleBias = 1;
  }

  int idx = 3;
  if (pNode.inputs().size() - m_DoScale - m_DoScaleBias == 3) {
    m_DoBias = 1;
    m_BiasIdx = idx++;
  }
  if (m_DoScale) {
    m_ScaleIdx = idx++;
    m_ConvOutputThreshold = pNode.f(onnx::Symbol("conv_output_threshold"));
  }
  if (m_DoScaleBias)
    m_ScaleBiasIdx = idx++;
}

TGConv *TGConv::addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                               MemOperand *pWeight, MemOperand *pBias,
                               MemOperand *pScale, MemOperand *pScaleBias)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pWeight);
  m_MemOperands.push_back(pOutput);
  if (pBias != nullptr) {
    assert(m_DoBias);
    m_MemOperands.push_back(pBias);
  }
  if (pScale != nullptr) {
    assert(m_DoScale);
    m_MemOperands.push_back(pScale);
  }
  if (pScaleBias != nullptr) {
    assert(m_DoScaleBias);
    m_MemOperands.push_back(pScaleBias);
  }
  return this;
}

void TGConv::print(OStream &pOS) const
{
  pOS << *m_MemOperands[2] << " = Conv <inN:" << m_InN << ", inC:" << m_InC
      << ", inH:" << m_InH << ", inW:" << m_InW << ", outC:" << m_OutC
      << ", groups:" << m_Groups << ", kH:" << m_KH << ", kW:" << m_KW
      << ", dilationH:" << m_DilationH << ", dilationW:" << m_DilationW
      << ", padH:" << (int)m_PadH << ", padW:" << (int)m_PadW
      << ", strideH:" << (int)m_StrideH << ", strideW:" << (int)m_StrideW
      << ", m_DoBias:" << m_DoBias << ", rShiftWidth:" << m_RShiftWidth << "> ("
      << *m_MemOperands[0] << ", " << *m_MemOperands[1];
  if (m_DoBias) {
    pOS << ", " << *m_MemOperands[3] << ")\n";
  } else {
    pOS << ")\n";
  }
}

void TGConv::emit() const
{
  DEBUG(print(dbgs()));
  float activation_arg[1] = { 0.0f };

  uint64_t biasAddr = m_DoBias ? m_MemOperands[m_BiasIdx]->m_Addr
                               : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_addr = m_DoScale ? m_MemOperands[m_ScaleIdx]->m_Addr
                                  : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_bias_addr = m_DoScaleBias
                                 ? m_MemOperands[m_ScaleBiasIdx]->m_Addr
                                 : bmnet::bmnet_asm::GADDR_INVALID;
  bmnet::bmnet_asm::bmnet_conv_parallel_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap
      m_MemOperands[2]->m_Addr,        // ofmap
      m_MemOperands[1]->m_Addr,        // weight
      biasAddr,                        // bias
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_mean,
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_variance,
      scale_addr,                      // ga_scale,
      scale_bias_addr,                 // ga_scale_bias,
      m_InN, m_InC, m_InH, m_InW, m_Groups, m_OutC, m_KH, m_KW, m_DilationH,
      m_DilationW, m_PadH, m_PadW, m_StrideH, m_StrideW, false, // result_add
      m_DoBias,                                                 // do_bias,
      0,                                                        // do_bn,
      m_DoScale, m_DoScaleBias, pm::match(m_pNode, pm::mTrueAttr("do_relu")),
      0,                  // bn_scale,
      0,                  // bn_eps,
      0,                  // activation_method,
      activation_arg,     // activation_arg[],
      0,                  // activation_ga_slope,
      0,                  // activation_channel_shared
      0,                  // activation_gt_rshift
      0,                  // activation_gt_rshift
      0,                  // activation_le_scale
      0,                  // activation_le_rshift
      m_RShiftWidth,      // right_shift_width
      0,                  // bn_right_shift_width
      m_ScaleRShiftWidth, // scale_right_shift_width
      0                   // use_winograd
  );
}

//#define DEBUG_WEIGHT_BIN
// TODO refactoring
void TGConv::prepareWeight(std::vector<int8_t> &pWeight)
{
  pWeight.clear();
  // m_MemOperands: ifmap, weight, ofmap, bias, scale, scale_bias
  // 8 bit weight
  {
    const onnx::Value *value = m_MemOperands[1]->m_Value;
    const onnx::Tensor &tensor =
        onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(m_MemOperands[1]->m_Type == onnx::TensorProto_DataType_INT8);
    const std::string &raw = tensor.raw();
    size_t count = onnc::getTotalCount(tensor.sizes());
    pWeight.resize(count);
    std::vector<int8_t> data;
    std::copy(raw.begin(), raw.end(), std::back_inserter(data));
#ifdef DEBUG_WEIGHT_BIN
    std::cout << value->uniqueName() << "\n";
    for (auto i : data) {
      std::cout << (int32_t)i << ",";
    }
    std::cout << "\n";
#endif
    assert((size_t)(m_OutC * m_InC * m_KH * m_KW / m_Groups) == count);

    // conv weight is arranged by (1, oc, kh*kw, ic)
    // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
    int ic = m_InC / m_Groups;
    for (int oc_i = 0; oc_i < m_OutC; ++oc_i) {
      for (int k_i = 0; k_i < m_KH * m_KW; ++k_i) {
        for (int ic_i = 0; ic_i < ic; ++ic_i) {
          pWeight[oc_i * (m_KH * m_KW * ic) + k_i * ic + ic_i] =
              data[oc_i * (m_KH * m_KW * ic) + ic_i * (m_KH * m_KW) + k_i];
        }
      }
    }
#ifdef DEBUG_WEIGHT_BIN
    std::cout << "after conv weight:\n";
    for (size_t j = 0; j < count; ++j) {
      std::cout << (int)weight[j] << ",";
    }
    std::cout << "\n";
#endif
  }

  // 16bit bias
  if (m_DoBias == 1) {
    auto *mem_op = m_MemOperands[m_BiasIdx];
    BM188xCodeEmitter::prepare16bitWeight(mem_op, pWeight);
  }

  // 8bit scale bias
  if (m_DoScale == 1) {
    auto *mem_op = m_MemOperands[m_ScaleIdx];
    BM188xCodeEmitter::prepare8bitWeight(mem_op, pWeight);
  }

  // 16bit scale bias
  if (m_DoScaleBias == 1) {
    auto *mem_op = m_MemOperands[m_ScaleBiasIdx];
    BM188xCodeEmitter::prepare16bitWeight(mem_op, pWeight);
  }
}

void TGConv::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ScaleRShiftWidth =
      pLayerCtable->convolution_param().scale_right_shift_width();
  // TODO add prelu_param
}

} // namespace BM188X
} // namespace onnc
