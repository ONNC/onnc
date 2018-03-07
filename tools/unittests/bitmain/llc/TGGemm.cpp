#include "TGGemm.h"
#include <bmkernel_api.h>

// TGGemm
TGGemm::TGGemm(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Gemm"), m_inputAddr(0x0), m_weightAddr(0x0),
      m_biasAddrl(0x0), m_outputAddr(0x0), m_inRow(0), m_inCol(0), m_outCol(0),
      m_haveBias(0), m_usingRelu(0), m_weightTp(false) {
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);

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

