#include "TGRelu.h"
#include <bmkernel_api.h>

TGRelu::TGRelu(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Relu"), m_negativeSlope(0) {
  const std::vector<onnx::Dimension> inDim = node.inputs()[0]->sizes();
  // TODO
  m_inputAddr = 0;
  m_outputAddr = 0;
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
}

void TGRelu::emit(void) const {
  std::cout << "TGRelu::emit\tm_inputAddr:" << m_inputAddr
            << " m_outputAddr:" << m_outputAddr
            << " m_negativeSlope:" << m_negativeSlope << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
  bmnet_relu_forward_bmkernel(m_inputAddr, m_outputAddr, m_negativeSlope, m_N,
                              m_C, m_H, m_W);
}
