#include "TGMaxPool.h"
#include <bmkernel_api.h>

using namespace onnc;

TGMaxPool::TGMaxPool(const ::onnx::Node &node, MemTable &memTable)
    : Operator(node, "MaxPool"), m_padH(0), m_padW(0), m_strideH(1),
      m_strideW(1) {

  auto inputs = node.inputs();
  auto outputs = node.outputs();
  m_inputAddr = memTable[inputs[0]->uniqueName()];
  m_outputAddr = memTable[outputs[0]->uniqueName()];

  const std::vector<::onnx::Dimension> inDim = node.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  m_kH = node.is(::onnx::Symbol("kernel_shape"))[0];
  m_kW = node.is(::onnx::Symbol("kernel_shape"))[1];

  // [leftPad, downPad, rightPad, upPad]
  if (node.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = node.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel padding on both ends
    m_padH = i[0];
    m_padW = i[1];
  }
  if (node.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = node.is(::onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
}

void TGMaxPool::emit(void) const
{
  std::cout << "TGMaxPool::emit\tm_inputAddr:" << m_inputAddr
            << " m_outputAddr:" << m_outputAddr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W
            << " m_kH:" << m_kH << " m_kW:" << m_kW << " m_padH:" << m_padH
            << " m_padW:" << m_padW << " m_srideH:" << m_strideH
            << " m_strideW:" << m_strideW << std::endl;

  // bmnet_pooling_forward_bmkernel
  bmnet::bmnet_pooling_forward_bmkernel(
                                 *tg_kernel::getInstance().ctx,
                                 m_inputAddr, m_outputAddr,
                                 GADDR_INVALID, // useless oindex_gaddr
                                 GADDR_INVALID, // useless relu_gaddr
                                 m_N, m_C, m_H, m_W, m_kH, m_kW, m_padH, m_padW,
                                 m_strideH, m_strideW, false, // is_avg_pooling
                                 0.0f,                        // always is 0.0f
                                 false // disable actvation
                                 );
}
