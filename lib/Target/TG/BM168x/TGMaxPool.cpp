#include "TGMaxPool.h"
#include "BM168xCodeEmitter.h"
#include <onnc/Target/TG/BM168x/bmkernel_api.h>

using namespace onnc;

TGMaxPool::TGMaxPool(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "MaxPool"), m_PadH(0), m_PadW(0), m_StrideH(1),
      m_StrideW(1)
{

  // auto inputs = pNode.inputs();
  // auto outputs = pNode.outputs();

  //// input
  // m_MemOperands.push_back(MemOperand(inputs[0]->uniqueName(),
  //                                   inputs[0]->sizes(),
  //                                   inputs[0]->elemType(), MemType::NEURON));
  //// output
  // m_MemOperands.push_back(MemOperand(outputs[0]->uniqueName(),
  //                                   outputs[0]->sizes(),
  //                                   outputs[0]->elemType(),
  //                                   MemType::NEURON));

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

void TGMaxPool::emit() const
{
  std::cout << "TGMaxPool::emit\tm_inputAddr:" << m_MemOperands[0]->m_Addr
            << " m_OutputAddr:" << m_MemOperands[1]->m_Addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W
            << " m_KH:" << m_KH << " m_KW:" << m_KW << " m_PadH:" << m_PadH
            << " m_PadW:" << m_PadW << " m_SrideH:" << m_StrideH
            << " m_StrideW:" << m_StrideW << std::endl;

  // bmnet_pooling_forward_bmkernel
  bmnet::bmnet_asm::bmnet_pooling_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr,
      bmnet::bmnet_asm::GADDR_INVALID, // useless oindex_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // useless relu_gaddr
      m_N, m_C, m_H, m_W, m_KH, m_KW, m_PadH, m_PadW, m_StrideH, m_StrideW,
      false, // is_avg_pooling
      0.0f,  // always is 0.0f
      false  // disable actvation
  );
}
