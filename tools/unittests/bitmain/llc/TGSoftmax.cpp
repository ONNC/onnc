#include "TGSoftmax.h"

#include <bmkernel_api.h>

TGSoftmax::TGSoftmax(const onnx::Node &node, MemTable &memTable)
    : TGOperator(node, "Softmax") {

  auto inputs = node.inputs();
  auto outputs = node.outputs();
  m_inputAddr = memTable[inputs[0]->uniqueName()];
  m_outputAddr = memTable[outputs[0]->uniqueName()];

  const std::vector<onnx::Dimension> inDim = node.inputs()[0]->sizes();
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

void TGSoftmax::emit(void) const {
  std::cout << "TGSoftmax::emit\tm_inputAddr:" << m_inputAddr
            << " m_outputAddr:" << m_outputAddr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
  bmnet_softmax_forward_bmkernel(m_inputAddr, m_outputAddr, m_N, m_C, m_H, m_W);
}
