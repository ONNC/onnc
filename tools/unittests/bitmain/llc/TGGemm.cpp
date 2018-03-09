#include "TGGemm.h"
#include <bmkernel_api.h>

void TGGemm::dumpOnnxConv(const onnx::Node &node) {

  std::cout << "dump TGGemm:" << std::endl;
  if (node.hasAttribute(onnx::Symbol("transA"))) {
    auto transA = node.i(onnx::Symbol("transA"));
    std::cout << "transA:" << transA << std::endl;
    assert(0 && "unimplemented attribute!");
  }

  if (node.hasAttribute(onnx::Symbol("transB"))) {
    auto transB = node.i(onnx::Symbol("transB"));
    std::cout << "transB:" << transB << std::endl;
    m_weightTp = true;
  }

  if (node.hasAttribute(onnx::Symbol("broadcast"))) {
    auto broadcast = node.i(onnx::Symbol("broadcast"));
    std::cout << "broadcast:" << broadcast << std::endl;
  }

  if (node.hasAttribute(onnx::Symbol("alpha"))) {
    auto alpha = node.i(onnx::Symbol("alpha"));
    std::cout << "alpha:" << alpha << std::endl;
    assert(0 && "unimplemented attribute!");
  }

  if (node.hasAttribute(onnx::Symbol("beta"))) {
    auto beta = node.i(onnx::Symbol("beta"));
    std::cout << "beta:" << beta << std::endl;
    assert(1.0 == beta && "unimplemented attribute!");
  }

  for (auto it = node.inputs().begin(), ie = node.inputs().end(); it != ie;
       ++it) {
    const onnx::Value *val = *it;
    std::cout << "input:" << val->uniqueName() << ":<";
    for (auto &dimension : val->sizes()) {
      std::cout << dimension.dim << ",";
    }
    std::cout << ">" << std::endl;
  }

  for (auto it = node.outputs().begin(), ie = node.outputs().end(); it != ie;
       ++it) {
    const onnx::Value *val = *it;
    std::cout << "output:" << val->uniqueName() << ":<";
    for (auto &dimension : val->sizes()) {
      std::cout << dimension.dim << ",";
    }
    std::cout << ">" << std::endl;
  }
}

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Gemm"), m_inputAddr(0x0), m_weightAddr(0x0),
      m_biasAddr(0x0), m_outputAddr(0x0), m_inRowNum(0), m_inColNum(0), m_outColNum(0),
      m_haveBias(0), m_usingRelu(0), m_weightTp(false) {
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);


  dumpOnnxConv(node);

  const std::vector<onnx::Dimension> aDim = node.inputs()[0]->sizes();
  const std::vector<onnx::Dimension> bDim = node.inputs()[1]->sizes();
  m_inRowNum = aDim[0].dim;
  m_inColNum = aDim[1].dim;
  m_outColNum = bDim[1].dim;
  m_haveBias = true;
  m_usingRelu = false;

  if (node.hasAttribute(onnx::Symbol("transB"))) {
    auto transB = node.i(onnx::Symbol("transB"));
    std::cout << "transB:" << transB << std::endl;
    m_weightTp = true;
  }
}

void TGGemm::emit(void) const {

  std::cout << "TGGemm::emit\tm_inputAddr:" << m_inputAddr
            << " m_weightAddr:" << m_weightAddr << " m_biasAddr:" << m_biasAddr
            << " m_outputAddr:" << m_outputAddr << " m_inRowNum:" << m_inRowNum
            << " m_inColNum:" << m_inColNum << " m_outColNum:" << m_outColNum
            << " m_haveBias:" << m_haveBias << " m_usingRelu:" << m_usingRelu
            << " m_weightTp:" << m_weightTp << std::endl;
  bmnet_fc_forward_bmkernel(m_inputAddr, m_weightAddr, m_biasAddr, m_outputAddr,
                            m_inRowNum, m_inColNum, m_outColNum, m_haveBias,
                            m_usingRelu, m_weightTp);
}
