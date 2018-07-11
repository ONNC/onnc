#include "TGLRN.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGLRN::TGLRN(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("LRN")), m_K(1)
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

TGLRN *TGLRN::addMemOperands(MemOperand *pInput, MemOperand *pSquLut,
                             MemOperand *pPowerLut, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pSquLut);
  m_MemOperands.push_back(pPowerLut);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGLRN::TGLRN::emit() const
{
  bmnet::bmnet_asm::bmnet_lrn_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input
      m_MemOperands[3]->m_Addr, // output
      m_MemOperands[1]->m_Addr, // sqr_lut
      m_MemOperands[2]->m_Addr, // power_lut,
      m_N, m_C, m_H, m_W, m_LocalSize, m_SumRightShiftWidth,
      m_LrnRightShiftWidth, m_ThresholdXQuantized);
}

} // namespace BM188X
} // namespace onnc
