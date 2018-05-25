#include "TGGemm.h"
#include "BM168xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_gemm"
#include <onnc/Support/Debug.h>

using namespace onnc;

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K
// X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const ::onnx::Node &pNode)
    : Operator(pNode, "Gemm"), m_inRowNum(0), m_inColNum(0), m_outColNum(0),
      m_haveBias(0), m_usingRelu(0), m_weightTp(false)
{
  auto inputs = pNode.inputs();
  auto outputs = pNode.outputs();

  // input
  m_MemOperands.push_back({ inputs[0]->uniqueName(), 0 });
  // weight
  m_MemOperands.push_back({ inputs[1]->uniqueName(), 0 });
  // bias
  m_MemOperands.push_back({ inputs[2]->uniqueName(), 0 });
  // output
  m_MemOperands.push_back({ outputs[0]->uniqueName(), 0 });

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
  DEBUG(dbgs() << "TGGemm::emit\tm_inputAddr:" << m_MemOperands[0].addr
               << " m_weightAddr:" << m_MemOperands[1].addr
               << " m_biasAddr:" << m_MemOperands[2].addr << " m_outputAddr:"
               << m_MemOperands[3].addr << " m_inRowNum:" << m_inRowNum
               << " m_inColNum:" << m_inColNum << " m_outColNum:" << m_outColNum
               << " m_haveBias:" << m_haveBias << " m_usingRelu:" << m_usingRelu
               << " m_weightTp:" << m_weightTp << std::endl;);
  bmnet::bmnet_fc_forward_bmkernel(
      *bm168x_kernel::getInstance().ctx, m_MemOperands[0].addr,
      m_MemOperands[1].addr, m_MemOperands[2].addr, m_MemOperands[3].addr,
      m_inRowNum, m_inColNum, m_outColNum, m_haveBias, m_usingRelu, m_weightTp);
}
