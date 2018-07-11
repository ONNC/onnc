#define DEBUG_TYPE "tg_sum"
#include "TGSum.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <bmnet/targets/plat-bm188x/bmkernel/bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace pm = onnc::PatternMatch;

namespace onnc {
namespace BM188X {

TGSum::TGSum(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Sum"))
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_InN = inDim[0].dim;
  m_InC = inDim[1].dim;
  m_InH = inDim[2].dim;
  m_InW = inDim[3].dim;
  m_InputNum = pNode.inputs().size();
  m_ThresholdXQuantized.resize(m_InputNum);
  m_DoRelu = pm::match(m_pNode, pm::mTrueAttr("do_relu"));
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
      << "inN:" << m_InN << ", inC:" << m_InC
      << ", inH:" << m_InH << ", inW:" << m_InW;

  int thx_size = m_InputNum;
  for (int i = 0; i < thx_size; ++i) {
    pOS << ", threshold_x_quantized[" << i << "] = "
        << m_ThresholdXQuantized[i] << "\n";
  }

  pOS << ", rShiftWidth:" << m_RShiftWidth << "> ("
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

  int in_size = m_MemOperands.size() - 1;
  uint64_t *input = new uint64_t[in_size];
  for (int i = 0; i < in_size; ++i)
    input[i] = m_MemOperands[i]->m_Addr;

  bmnet::bmnet_asm::bmnet_eltwise_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_CTX,
      input,                        // inputs
      m_MemOperands.back()->m_Addr, // ouput
      m_InputNum,
      1, // op: SUM
      m_InN, m_InC, m_InH, m_InW,
      m_DoRelu,      // do_relu
      0.0,           // relu_slope,
      m_RShiftWidth, // right_shift_width
      m_ThresholdXQuantized.data());

  delete[] input;
}

void TGSum::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  for (size_t i = 0; i < m_InputNum; ++i) {
    m_ThresholdXQuantized[i] = pLayerCtable->threshold_x_quantized(i);
  }
}

} // namespace BM188X
} // namespace onnc
