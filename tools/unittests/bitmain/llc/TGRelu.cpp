#include "TGRelu.h"
#include <bmkernel_api.h>

TGRelu::TGRelu(const onnx::Node &node, MemTable &memTable)
    : Operator(node, "Relu"), m_negativeSlope(0) {
  const std::vector<onnx::Dimension> inDim = node.inputs()[0]->sizes();

  auto inputs = node.inputs();
  auto outputs = node.outputs();
  m_inputAddr = memTable[inputs[0]->uniqueName()];
  m_outputAddr = memTable[outputs[0]->uniqueName()];

  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = inDim[2].dim;
    m_W = inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = 1;
    m_C = 1;
    m_H = inDim[0].dim;
    m_W = inDim[1].dim;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }

}

void TGRelu::emit(void) const {
  std::cout << "TGRelu::emit\tm_inputAddr:" << m_inputAddr
            << " m_outputAddr:" << m_outputAddr
            << " m_negativeSlope:" << m_negativeSlope << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
  bmnet_relu_forward_bmkernel(m_inputAddr, m_outputAddr, m_negativeSlope, m_N,
                              m_C, m_H, m_W);
}
