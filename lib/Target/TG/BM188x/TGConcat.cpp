#define DEBUG_TYPE "tg_concat"
#include "TGConcat.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGConcat::TGConcat(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Concat")), m_NeedQuantizeNum(0),
      m_RShiftWidth(pNode.inputs().size(), 0),
      m_ThresholdXQuantized(pNode.inputs().size(), 1)
{
  m_ConcatAxis = pNode.i(::onnx::Symbol("axis"));
  for (size_t i = 0; i < pNode.inputs().size(); i++) {
    const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[i]->sizes();
    m_InputDims.push_back(inDim[m_ConcatAxis].dim);
  }

  for (size_t i = 0; i < pNode.outputs()[0]->sizes().size(); i++) {
    const std::vector< ::onnx::Dimension> outDim = pNode.outputs()[0]->sizes();
    m_OutputDim.push_back(outDim[i].dim);
  }
}

TGConcat *TGConcat::addMemOperands(std::vector<MemOperand *> &pInput,
                                   MemOperand *pOutput)
{
  for (size_t i = 0; i < pInput.size(); i++) {
    m_MemOperands.push_back(pInput[i]);
    m_InputAddr.push_back(pInput[i]->m_Addr);
  }
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGConcat::emit() const
{
  // Need to modify the api to use const.
  std::vector<uint64_t> input_addr;
  for (size_t i = 0; i < m_InputDims.size(); i++)
    input_addr.push_back(m_MemOperands[i]->m_Addr);

  bmnet::bmnet_asm::bmnet_concat_fixed_forward_bmkernel(
      input_addr.data(), m_MemOperands.back()->m_Addr,
      const_cast<int *>(m_InputDims.data()), m_InputDims.size(), m_ConcatAxis,
      m_OutputDim.size(), const_cast<int *>(m_OutputDim.data()),
      m_NeedQuantizeNum,
      m_RShiftWidth.data(),        // right_shift_width
      m_ThresholdXQuantized.data() // threshold_x_quantized
  );
}

void TGConcat::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_NeedQuantizeNum = pLayerCtable->concat_param().need_quantize_num();
  if (m_NeedQuantizeNum) {
    assert(pLayerCtable->concat_param().right_shift_width_size() ==
           pLayerCtable->concat_param().threshold_x_quantized_size());
    for (int i = 0; i < pLayerCtable->concat_param().right_shift_width_size();
         i++) {
      m_RShiftWidth[i] = pLayerCtable->concat_param().right_shift_width(i);
      m_ThresholdXQuantized[i] =
          pLayerCtable->concat_param().threshold_x_quantized(i);
    }
  }
}

} // namespace BM188X
} // namespace onnc
