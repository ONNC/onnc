#define DEBUG_TYPE "tg_gemm"

#include "TGGemm.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K
// X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const ::onnx::Node &pNode,
               const LayerCalibrationParameter &pLayerCtable)
    : ComputeOperand2(pNode, "Gemm"), m_inRowNum(0), m_inColNum(0),
      m_outColNum(0), m_haveBias(0), m_weightTp(false),
      m_LayerCtable(pLayerCtable)
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

  if (pNode.hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = pNode.i(::onnx::Symbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_weightTp = true;
  }
}

void TGGemm::print(OStream &pOS) const
{
  pOS << m_MemOperands[3] << " = Gemm <inRowNum:" << m_inRowNum
      << ", inColNum:" << m_inColNum << ", outColNum:" << m_outColNum
      << ", m_weightTp:" << m_weightTp << "> (" << m_MemOperands[0] << ", "
      << m_MemOperands[1] << ", " << m_MemOperands[2] << ")\n";
}

void TGGemm::emit() const
{
  DEBUG(print(dbgs()));
  int rShiftWidth = m_LayerCtable.right_shift_width();
  bmnet::bmnet_fc_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx,
      m_MemOperands[0].addr, // input_data_gaddr
      m_MemOperands[1].addr, // weight_data_gaddr
      m_MemOperands[2].addr, // bias_data_gaddr
      m_MemOperands[3].addr, // output_data_gaddr
      m_inRowNum,            // input_row_num
      m_inColNum,            // input_col_num
      m_outColNum,           // weight_col_num
      m_haveBias,            // have_bias
      0,                     // do_activation
      0,                     // activation_method
      GADDR_INVALID,         // activation_ga_slope
      0,                     // activation_channel_shared
      0,                     // activation_gt_scale
      0,                     // activation_gt_rshift
      0,                     // activation_le_scale
      0,                     // activation_le_rshift
      m_weightTp,            // weight_transpose
      0,                     // left_shift_width //TODO
      rShiftWidth            // right_shift_width
  );
}

} // namespace BM188X
} // namespace onnc
