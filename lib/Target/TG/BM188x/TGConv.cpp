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
    : ComputeOperator2(pNode, "Conv"), m_groups(1), m_dilationH(1),
      m_dilationW(1), m_padH(0), m_padW(0), m_strideH(1), m_strideW(1),
      m_doBias(0), m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_inN = inDim[0].dim;
  m_inC = inDim[1].dim;
  m_inH = inDim[2].dim;
  m_inW = inDim[3].dim;
  const std::vector< ::onnx::Dimension> weightDim = pNode.inputs()[1]->sizes();
  m_outC = weightDim[0].dim;
  if (pNode.hasAttribute(::onnx::Symbol("group"))) {
    m_groups = pNode.i(::onnx::Symbol("group"));
  }
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    m_kH = i[0];
    m_kW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("dilations"))) {
    auto &i = pNode.is(::onnx::Symbol("dilations"));
    m_dilationH = i[0];
    m_dilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_padH = i[0];
    m_padW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
  if (3 == pNode.inputs().size()) {
    m_doBias = 1;
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
  pOS << *m_MemOperands[2] << " = Conv <inN:" << m_inN << ", inC:" << m_inC
      << ", inH:" << m_inH << ", inW:" << m_inW << ", outC:" << m_outC
      << ", groups:" << m_groups << ", kH:" << m_kH << ", kW:" << m_kW
      << ", dilationH:" << m_dilationH << ", dilationW:" << m_dilationW
      << ", padH:" << (int)m_padH << ", padW:" << (int)m_padW
      << ", strideH:" << (int)m_strideH << ", strideW:" << (int)m_strideW
      << ", m_doBias:" << m_doBias
      << ", rShiftWidth:" << m_LayerCtable.right_shift_width() << "> ("
      << *m_MemOperands[0] << ", " << *m_MemOperands[1] << ", "
      << *m_MemOperands[3] << ")\n";
}

void TGConv::emit() const
{
  DEBUG(print(dbgs()));

  int rShiftWidth = m_LayerCtable.right_shift_width();

  bmnet::bmnet_conv_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx, m_MemOperands[0]->addr, // ifmap
      m_MemOperands[2]->addr,                                      // ofmap
      m_MemOperands[1]->addr,                                      // weight
      m_MemOperands[3]->addr,                                      // bias
      GADDR_INVALID, // ga_bn_mean,
      GADDR_INVALID, // ga_bn_variance,
      GADDR_INVALID, // ga_scale,
      GADDR_INVALID, // ga_scale_bias,
      m_inN, m_inC, m_inH, m_inW, m_groups, m_outC, m_kH, m_kW, m_dilationH,
      m_dilationW, m_padH, m_padW, m_strideH, m_strideW, false, // result_add
      m_doBias,                                                 // do_bias,
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
void TGConv::prepareWeight(std::vector<int8_t> &weight)
{
  weight.clear();
  // m_MemOperands: ifmap, weight, ofmap, bias
  // 8 bit weight
  {
    const ::onnx::Value *value = m_MemOperands[1]->value;
    const ::onnx::Tensor &tensor =
        ::onnc::getTensor(value->uniqueName(), *value->owningGraph());
    assert(tensor.elem_type() == ::onnx::TensorProto_DataType_INT8);
    const std::string &raw = tensor.raw();
    size_t count = ::onnc::getTotalCount(tensor.sizes());
    weight.resize(count);
    std::vector<int8_t> data;
    std::copy(raw.begin(), raw.end(), std::back_inserter(data));
#ifdef DEBUG_WEIGHT_BIN
    std::cout << value->uniqueName() << "\n";
    for (auto i : data) {
      std::cout << (int32_t)i << ",";
    }
    std::cout << "\n";
#endif
    assert((size_t)(m_outC * m_inC * m_kH * m_kW) == count);

    // conv weight is arranged by (1, oc, kh*kw, ic)
    // convert (oc, ic, kh, kw) to (1, oc, kh*kw, ic)
    for (int oc_i = 0; oc_i < m_outC; ++oc_i) {
      for (int k_i = 0; k_i < m_kH * m_kW; ++k_i) {
        for (int ic_i = 0; ic_i < m_inC; ++ic_i) {
          weight[oc_i * (m_kH * m_kW * m_inC) + k_i * m_inC + ic_i] =
              data[oc_i * (m_kH * m_kW * m_inC) + ic_i * (m_kH * m_kW) + k_i];
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
    const ::onnx::Value *value = m_MemOperands[3]->value;
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
    size_t offset = weight.size();
    weight.resize(offset + count * 2);
    for (size_t i = 0; i < count; ++i) {
      weight[offset + i] = (int8_t)(int16_vector[i] & 0xff);
      weight[offset + i + count] = (int8_t)((int16_vector[i] >> 8) & 0xff);
    }
  }
}

} // namespace BM188X
} // namespace onnc
