#include "TGGemm.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_gemm"
#include <onnc/Support/Debug.h>

using namespace onnc;

void TGGemm::dumpOnnxGemm(const ::onnx::Node &pNode)
{
  DEBUG(dbgs() << "dump TGGemm:" << std::endl;);
  if (pNode.hasAttribute(::onnx::Symbol("transA"))) {
    auto transA = pNode.i(::onnx::Symbol("transA"));
    DEBUG(dbgs() << "transA:" << transA << std::endl;);
    assert(0 && "unimplemented attribute!");
  }

  if (pNode.hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = pNode.i(::onnx::Symbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_weightTp = true;
  }

  if (pNode.hasAttribute(::onnx::Symbol("broadcast"))) {
    auto broadcast = pNode.i(::onnx::Symbol("broadcast"));
    DEBUG(dbgs() << "broadcast:" << broadcast << std::endl;);
  }

  if (pNode.hasAttribute(::onnx::Symbol("alpha"))) {
    auto alpha = pNode.i(::onnx::Symbol("alpha"));
    DEBUG(dbgs() << "alpha:" << alpha << std::endl;);
    assert(0 && "unimplemented attribute!");
  }

  if (pNode.hasAttribute(::onnx::Symbol("beta"))) {
    auto beta = pNode.i(::onnx::Symbol("beta"));
    DEBUG(dbgs() << "beta:" << beta << std::endl;);
    assert(1.0 == beta && "unimplemented attribute!");
  }

  for (auto it = pNode.inputs().begin(), ie = pNode.inputs().end(); it != ie;
       ++it) {
    const ::onnx::Value *val = *it;
    DEBUG(dbgs() << "input:" << val->uniqueName() << ":<";);
    for (auto &dimension : val->sizes()) {
      DEBUG(dbgs() << dimension.dim << ",";);
    }
    DEBUG(dbgs() << ">" << std::endl;);
  }

  for (auto it = pNode.outputs().begin(), ie = pNode.outputs().end(); it != ie;
       ++it) {
    const ::onnx::Value *val = *it;
    DEBUG(dbgs() << "output:" << val->uniqueName() << ":<";);
    for (auto &dimension : val->sizes()) {
      DEBUG(dbgs() << dimension.dim << ",";);
    }
    DEBUG(dbgs() << ">" << std::endl;);
  }
}

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K
// X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const ::onnx::Node &pNode)
    : Operator(pNode, "Gemm"), m_inputAddr(0x0), m_weightAddr(0x0),
      m_biasAddr(0x0), m_outputAddr(0x0), m_inRowNum(0), m_inColNum(0),
      m_outColNum(0), m_haveBias(0), m_usingRelu(0), m_weightTp(false)
{

  dumpOnnxGemm(pNode);

  auto inputs = pNode.inputs();
  auto outputs = pNode.outputs();
  const std::vector< ::onnx::Dimension> aDim = pNode.inputs()[0]->sizes();
  const std::vector< ::onnx::Dimension> bDim = pNode.outputs()[0]->sizes();
  m_inRowNum = aDim[0].dim;
  m_inColNum = aDim[1].dim;
  if (aDim.size() == 4) {
    m_inColNum *= aDim[2].dim * aDim[3].dim;
  }
  m_outColNum = bDim[1].dim;
  m_haveBias = true;
  m_usingRelu = false;

  if (pNode.hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = pNode.i(::onnx::Symbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_weightTp = true;
  }
}

void TGGemm::emit() const
{
  DEBUG(dbgs() << "TGGemm::emit\tm_inputAddr:" << m_inputAddr
               << " m_weightAddr:" << m_weightAddr << " m_biasAddr:"
               << m_biasAddr << " m_outputAddr:" << m_outputAddr
               << " m_inRowNum:" << m_inRowNum << " m_inColNum:" << m_inColNum
               << " m_outColNum:" << m_outColNum << " m_haveBias:" << m_haveBias
               << " m_usingRelu:" << m_usingRelu << " m_weightTp:" << m_weightTp
               << std::endl;);
#if 0
  bmnet::bmnet_fc_forward_bmkernel(*bm1880_kernel::getInstance().m_Ctx, m_inputAddr,
                                   m_weightAddr, m_biasAddr, m_outputAddr,
                                   m_inRowNum, m_inColNum, m_outColNum,
                                   m_haveBias, m_usingRelu, m_weightTp);
#endif
}
