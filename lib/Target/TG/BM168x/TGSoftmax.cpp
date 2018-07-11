#include "TGSoftmax.h"
#include "BM168xCodeEmitter.h"
#include <bmnet/targets/plat-bm168x/bmkernel/bmkernel_api.h>

using namespace onnc;

TGSoftmax::TGSoftmax(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "Softmax")
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
  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = 1;
    m_W = inDim[2].dim * inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = 1;
    m_W = 1;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }
}

void TGSoftmax::emit() const
{
  std::cout << "TGSoftmax::emit\tm_inputAddr:" << m_MemOperands[0]->m_Addr
            << " m_OutputAddr:" << m_MemOperands[1]->m_Addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
  bmnet::bmnet_softmax_forward_bmkernel(
      *bm168x_kernel::getInstance().m_CTX, m_MemOperands[0]->m_Addr,
      m_MemOperands[1]->m_Addr, m_N, m_C, m_H, m_W);
}
