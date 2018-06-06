#include "TGConv.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

// TGConv
TGConv::TGConv(const ::onnx::Node &pNode,
               const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
    : ComputeOperator2(pNode, "Conv"), m_Groups(1), m_DilationH(1),
      m_DilationW(1), m_PadH(0), m_PadW(0), m_StrideH(1), m_StrideW(1),
      m_DoBias(0), m_LayerCtable(pLayerCtable)
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
  if (3 == pNode.inputs().size()) {
    m_DoBias = 1;
  }
}

TGConv *TGConv::addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                               MemOperand *pWeight, MemOperand *pBias)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pWeight);
  m_MemOperands.push_back(pOutput);
  m_MemOperands.push_back(pBias);
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
      << ", m_DoBias:" << m_DoBias
      << ", rShiftWidth:" << m_LayerCtable.right_shift_width() << "> ("
      << *m_MemOperands[0] << ", " << *m_MemOperands[1] << ", "
      << *m_MemOperands[3] << ")\n";
}

void TGConv::emit() const
{
  DEBUG(print(dbgs()));

  int rShiftWidth = m_LayerCtable.right_shift_width();

  bmnet::bmnet_conv_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX, m_MemOperands[0]->m_Addr, // ifmap
      m_MemOperands[2]->m_Addr,                                      // ofmap
      m_MemOperands[1]->m_Addr,                                      // weight
      m_MemOperands[3]->m_Addr,                                      // bias
      GADDR_INVALID, // ga_bn_mean,
      GADDR_INVALID, // ga_bn_variance,
      GADDR_INVALID, // ga_scale,
      GADDR_INVALID, // ga_scale_bias,
      m_InN, m_InC, m_InH, m_InW, m_Groups, m_OutC, m_KH, m_KW, m_DilationH,
      m_DilationW, m_PadH, m_PadW, m_StrideH, m_StrideW, false, // result_add
      m_DoBias,                                                 // do_bias,
      0,                                                        // do_bn,
      0,                                                        // do_scale,
      0,           // do_scale_bias,
      0,           // do_activation,
      0,           // bn_scale,
      0,           // bn_eps,
      0,           // activation_method,
      nullptr,     // activation_arg[],
      0,           // activation_ga_slope,
      0,           // activation_channel_shared
      0,           // activation_gt_rshift
      0,           // activation_gt_rshift
      0,           // activation_le_scale
      0,           // activation_le_rshift
      rShiftWidth, // right_shift_width
      0,           // bn_right_shift_width
      0,           // scale_right_shift_width
      0            // use_winograd
  );
}

//#define DEBUG_WEIGHT_BIN
void TGConv::prepareWeight(std::vector<int8_t> &pWeight)
{
  pWeight.clear();
  // m_MemOperands: ifmap, weight, ofmap, bias
  // 8 bit weight
  {
    const ::onnx::Value *value = m_MemOperands[1]->m_Value;
    const ::onnx::Tensor &tensor =
        ::onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(tensor.elem_type() == ::onnx::TensorProto_DataType_INT8);
    const std::string &raw = tensor.raw();
    size_t count = ::onnc::getTotalCount(tensor.sizes());
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
    assert((size_t)(m_OutC * m_InC * m_KH * m_KW) == count);

    // conv weight is arranged by (1, oc, kh*kw, ic)
    // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
    for (int oc_i = 0; oc_i < m_OutC; ++oc_i) {
      for (int k_i = 0; k_i < m_KH * m_KW; ++k_i) {
        for (int ic_i = 0; ic_i < m_InC; ++ic_i) {
          pWeight[oc_i * (m_KH * m_KW * m_InC) + k_i * m_InC + ic_i] =
              data[oc_i * (m_KH * m_KW * m_InC) + ic_i * (m_KH * m_KW) + k_i];
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
  {
    const ::onnx::Value *value = m_MemOperands[3]->m_Value;
    const ::onnx::Tensor &tensor =
        ::onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(tensor.elem_type() == ::onnx::TensorProto_DataType_INT16);
    size_t count = ::onnc::getTotalCount(tensor.sizes());
    std::vector<int16_t> int16_vector(count);
    memcpy(int16_vector.data(), tensor.raw().data(), count * sizeof(int16_t));
#ifdef DEBUG_WEIGHT_BIN
    std::cout << value->uniqueName() << "\n";
    for (auto i : int16_vector) {
      std::cout << i << ",";
    }
    std::cout << "\n";
#endif
    size_t offset = pWeight.size();
    pWeight.resize(offset + count * 2);
    for (size_t i = 0; i < count; ++i) {
      pWeight[offset + i] = (int8_t)(int16_vector[i] & 0xff);
      pWeight[offset + i + count] = (int8_t)((int16_vector[i] >> 8) & 0xff);
    }
  }
}

void TGConv::toASM(tg::bm1880::Insn *pI) const
{
  pI->set_name(getLayerName());
  pI->set_type(tg::bm1880::Insn::Convolution);
  {
    auto *conv = pI->mutable_convolution_param();
    conv->set_group(m_Groups);
    conv->set_do_bias(m_DoBias);
    {
      auto *input = conv->mutable_input();
      bm_asm::setDim(input, m_InN, m_InC, m_InH, m_InW);
      bm_asm::setMem(input, m_MemOperands.at(0), tg::bm1880::Operand::Int8);
    }
    {
      auto *output = conv->mutable_output();
      bm_asm::setMem(output, m_MemOperands.at(2), tg::bm1880::Operand::Int8);
    }
    {
      auto *weight = conv->mutable_weight();
      weight->set_c(m_OutC);
      bm_asm::setMem(weight, m_MemOperands.at(1), tg::bm1880::Operand::Int8);
    }
    {
      auto *bias = conv->mutable_bias();
      bias->set_c(m_OutC);
      bm_asm::setMem(bias, m_MemOperands.at(3), tg::bm1880::Operand::Int16);
    }
    bm_asm::setDim(conv->mutable_kernel(), m_KH, m_KW);
    bm_asm::setDim(conv->mutable_pad(), m_PadH, m_PadW);
    bm_asm::setDim(conv->mutable_stride(), m_StrideH, m_StrideW);
    bm_asm::setDim(conv->mutable_dilation(), m_DilationH, m_DilationW);
    {
      auto *cal = conv->mutable_ctable();
      cal->set_right_shift_width(m_LayerCtable.right_shift_width());
    }
  }
}

} // namespace BM188X
} // namespace onnc
