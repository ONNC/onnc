#include "TGSoftmax.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

using namespace onnc;

TGSoftmax::TGSoftmax(const ::onnx::Node &pNode) : Operator(pNode, "Softmax")
{

  auto inputs = pNode.inputs();
  auto outputs = pNode.outputs();

  // input
  m_MemOperands.push_back({ inputs[0]->uniqueName(), 0 });
  // output
  m_MemOperands.push_back({ outputs[0]->uniqueName(), 0 });

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
  std::cout << "TGSoftmax::emit\tm_inputAddr:" << m_MemOperands[0].addr
            << " m_outputAddr:" << m_MemOperands[1].addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
#if 0
  bmnet::bmnet_softmax_forward_bmkernel(
          *bm1880_kernel::getInstance().m_Ctx,
          m_inputAddr, m_outputAddr, m_N, m_C, m_H, m_W);
#endif
}
