#include "TGSum.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_sum"
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGSum::TGSum(const ::onnx::Node &pNode,
             const tg::bm1880::LayerCalibrationParameter &pLayerCtable)
    : ComputeOperator2(pNode, "Sum"), m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_inN = inDim[0].dim;
  m_inC = inDim[1].dim;
  m_inH = inDim[2].dim;
  m_inW = inDim[3].dim;
  m_inSize = m_inN * m_inC * m_inH * m_inW;
}

TGSum *TGSum::addMemOperands(std::vector<MemOperand *> pInput,
                             MemOperand *pOutput)
{
  for (auto in : pInput)
    m_MemOperands.push_back(in);
  m_MemOperands.push_back(pOutput);
  return this;
}

// clang-format off
void TGSum::print(OStream &pOS) const
{
  pOS << *(m_MemOperands.back()) << " = Sum "
      << "< "
      << "inN:" << m_inN << ", inC:" << m_inC
      << ", inH:" << m_inH << ", inW:" << m_inW;

  int thx_size = m_LayerCtable.threshold_x_quantized_size();
  for (int i = 0; i < thx_size; ++i) {
    pOS << ", threshold_x_quantized[" << i << "] = "
        << m_LayerCtable.threshold_x_quantized(i) << "\n";
  }

  pOS << ", rShiftWidth:" << m_LayerCtable.right_shift_width() << "> ("
      << " >";

  pOS << " (";
  int mop_size = m_MemOperands.size();
  for (int i = 0; i < mop_size; ++i)
    pOS << *m_MemOperands[i] << ", ";
  pOS << ")\n";
}
// clang-format on

void TGSum::emit() const
{
  DEBUG(print(dbgs()));

  int rShiftWidth = m_LayerCtable.right_shift_width();
  int in_size = m_MemOperands.size() - 1;
  uint64_t *input = new uint64_t[in_size];
  for (int i = 0; i < in_size; ++i)
    input[i] = m_MemOperands[i]->addr;

  bmnet::bmnet_eltwise_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx,
      input,                      // inputs
      m_MemOperands.back()->addr, // ouput
      m_inSize,
      1, // op: SUM
      m_inN, m_inC, m_inH, m_inW,
      false,       // do_relu
      0.0,         // relu_slope,
      rShiftWidth, // right_shift_width
      m_LayerCtable.threshold_x_quantized().data());

  delete input;
}

} // namespace BM188X
} // namespace onnc
