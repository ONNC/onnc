#define DEBUG_TYPE "tg_relu"
#include "TGRelu.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGRelu::TGRelu(const ::onnx::Node &pNode,
               const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
    : ComputeOperator2(pNode, "Relu"), m_NegativeSlope(0),
      m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = inDim[2].dim;
    m_W = inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = inDim[0].dim;
    m_C = 1;
    m_H = inDim[1].dim;
    m_W = 1;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }
}

TGRelu *TGRelu::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGRelu::print(OStream &pOS) const
{
  pOS << *m_MemOperands[1] << " = ReLU <negativeSlope:" << m_NegativeSlope
      << ", N:" << m_N << ", C:" << m_C << ", H:" << m_H << ", W:" << m_W
      << "> (" << *m_MemOperands[1] << ")\n";
}

void TGRelu::emit() const
{
  DEBUG(print(dbgs()));

  bmnet::bmnet_relu_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX,
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // output_gaddr
      m_NegativeSlope,          // negative_slope
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W                       // input_w
  );
}
void TGRelu::toASM(tg::bm1880::Insn *pI) const
{
  pI->set_name(getLayerName());
  pI->set_type(tg::bm1880::Insn::ReLU);
  {
    auto *relu = pI->mutable_relu_param();
    relu->set_negative_slope(m_NegativeSlope);
    {
      auto *input = relu->mutable_input();
      bm_asm::setDim(input, m_N, m_C, m_H, m_W);
      bm_asm::setMem(input, m_MemOperands.at(0), tg::bm1880::Operand::Int8);
    }
    {
      auto *output = relu->mutable_output();
      bm_asm::setMem(output, m_MemOperands.at(1), tg::bm1880::Operand::Int8);
    }
  }
}

} // namespace BM188X
} // namespace onnc
