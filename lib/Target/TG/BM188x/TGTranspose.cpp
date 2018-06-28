#define DEBUG_TYPE "tg_transpose"
#include "TGTranspose.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGTranspose::TGTranspose(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Transpose")),
      m_NeedPermute(false)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;
  m_OutputShape.resize(inDim.size());

  auto &orders = pNode.is(::onnx::Symbol("perm"));
  assert(orders.size() == 4);
  m_Order.resize(orders.size());
  assert(((orders[0] == 0) && (orders[1] == 2) && (orders[2] == 3) &&
          (orders[3] == 1)) ||
         ((orders[0] == 2) && (orders[1] == 3) && (orders[2] == 0) &&
          (orders[3] == 1)) ||
         ((orders[0] == 1) && (orders[1] == 0) && (orders[2] == 2) &&
          (orders[3] == 3)));

  for (u32 i = 0; i < orders.size(); ++i) {
    m_Order[i] = orders[i];
    m_OutputShape[i] = inDim[orders[i]].dim;
  }

  // Check if we need to reorder the data or keep it.
  for (u32 i = 0; i < orders.size(); ++i) {
    if (orders[i] != i) {
      // As long as there is one order which is different from the natural order
      // of the data, we need to permute. Otherwise, we share the data and diff.
      m_NeedPermute = true;
      break;
    }
  }
}

TGTranspose *TGTranspose::addMemOperands(MemOperand *pInput,
                                         MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGTranspose::print(OStream &pOS) const
{
  pOS << *m_MemOperands[1] << " = Transpose <N:" << m_N << ", C:" << m_C
      << ", H:" << m_H << ", W:" << m_W << ",  OrderN:" << m_Order[0]
      << ", OrderC:" << m_Order[1] << ", OrderH:" << m_Order[2]
      << ", OrderW:" << m_Order[3] << ",  ON:" << m_OutputShape[0]
      << ", OC:" << m_OutputShape[1] << ", OH:" << m_OutputShape[2]
      << ", OW:" << m_OutputShape[3] << "> (" << *m_MemOperands[0] << ")\n";
}

void TGTranspose::emit() const
{
  DEBUG(print(dbgs()));
  bmnet::bmnet_permute_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX, m_MemOperands[0]->m_Addr,
      m_MemOperands[1]->m_Addr, m_N, m_C, m_H, m_W, m_OutputShape[0],
      m_OutputShape[1], m_OutputShape[2], m_OutputShape[3], m_Order[0],
      m_Order[1], m_Order[2], m_Order[3], m_NeedPermute);
}

void TGTranspose::toASM(tg::bm1880::Inst *pI) const
{
  pI->set_name(getLayerName());
  pI->set_type("bmnet_permute_fixed_forward_bmkernel");
  {
    auto *permute = pI->mutable_permute();
    permute->set_input_gaddr(m_MemOperands[0]->m_Addr);
    permute->set_output_gaddr(m_MemOperands[1]->m_Addr);
    permute->set_input_n(m_N);
    permute->set_input_c(m_C);
    permute->set_input_h(m_H);
    permute->set_input_w(m_W);
    permute->set_output_n(m_OutputShape[0]);
    permute->set_output_c(m_OutputShape[1]);
    permute->set_output_h(m_OutputShape[2]);
    permute->set_output_w(m_OutputShape[3]);
    permute->set_order_n(m_Order[0]);
    permute->set_order_c(m_Order[1]);
    permute->set_order_h(m_Order[2]);
    permute->set_order_w(m_Order[3]);
    permute->set_need_permute(m_NeedPermute);
  }
}
} // namespace BM188X
} // namespace onnc
