#include "TGMaxPool.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

using namespace onnc;

TGMaxPool::TGMaxPool(const ::onnx::Node &pNode)
    : ComputeOperand2(pNode, "MaxPool"), m_padH(0), m_padW(0), m_strideH(1),
      m_strideW(1)
{

  auto inputs = pNode.inputs();
  auto outputs = pNode.outputs();

  // input
  m_MemOperands.push_back({ inputs[0]->uniqueName(), 0 });
  // output
  m_MemOperands.push_back({ outputs[0]->uniqueName(), 0 });

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

void TGMaxPool::emit() const
{
  std::cout << "TGMaxPool::emit\tm_inputAddr:" << m_MemOperands[0].addr
            << " m_outputAddr:" << m_MemOperands[1].addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W
            << " m_kH:" << m_kH << " m_kW:" << m_kW << " m_padH:" << m_padH
            << " m_padW:" << m_padW << " m_srideH:" << m_strideH
            << " m_strideW:" << m_strideW << std::endl;
#if 0
  // bmnet_pooling_forward_bmkernel
  bmnet::bmnet_pooling_forward_bmkernel(
                                 *bm1880_kernel::getInstance().m_Ctx,
                                 m_inputAddr, m_outputAddr,
                                 GADDR_INVALID, // useless oindex_gaddr
                                 GADDR_INVALID, // useless relu_gaddr
                                 m_N, m_C, m_H, m_W, m_kH, m_kW, m_padH, m_padW,
                                 m_strideH, m_strideW, false, // is_avg_pooling
                                 0.0f,                        // always is 0.0f
                                 false // disable actvation
                                 );
#endif
}
