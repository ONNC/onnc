#include "TGLRN.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGLRN::TGLRN(const ::onnx::Node &pNode,
             const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
    : ComputeOperator2(pNode, "LRN"), m_K(1)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  m_Alpha = pNode.f(::onnx::Symbol("alpha"));
  m_Beta = pNode.f(::onnx::Symbol("beta"));
  if (pNode.hasAttribute(::onnx::Symbol("bias"))) {
    m_K = pNode.f(::onnx::Symbol("bias"));
  }
  m_LocalSize = pNode.i(::onnx::Symbol("size"));
}

void TGLRN::TGLRN::emit() const
{
  std::cout << "TGLRUN::emit\tm_inputAddr:" << m_MemOperands[0]->m_Addr
            << " m_OutputAddr:" << m_MemOperands[1]->m_Addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W
            << " m_Alpha:" << m_Alpha << "m_LocalSize:" << m_LocalSize
            << " m_Beta:" << m_Beta << "m_K:" << m_K << std::endl;
#if 0
  bmnet::bmnet_lrn_forward_bmkernel(
                             *bm1880_kernel::getInstance().m_CTX,
                             m_InputAddr, m_OutputAddr, m_N, m_C, m_H, m_W,
                             m_Alpha, m_LocalSize, m_Beta, m_K);
#endif
}

void TGLRN::toASM(tg::bm1880::Insn *pI) const
{
  pI->set_name(getLayerName());
  pI->set_type(tg::bm1880::Insn::LRN);
  {
    auto *lrn = pI->mutable_lrn_param();
    lrn->set_alpha(m_Alpha);
    lrn->set_beta(m_Beta);
    {
      auto *input = lrn->mutable_input();
      bm_asm::setDim(input, m_N, m_C, m_H, m_W);
      bm_asm::setMem(input, m_MemOperands.at(0), tg::bm1880::Operand::Int8);
    }
    {
      auto *output = lrn->mutable_output();
      bm_asm::setMem(output, m_MemOperands.at(1), tg::bm1880::Operand::Int8);
    }
  }
}

} // namespace BM188X
} // namespace onnc
