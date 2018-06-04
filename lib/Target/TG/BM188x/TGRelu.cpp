#define DEBUG_TYPE "tg_relu"
#include "TGRelu.h"
#include "BM188xCodeEmitter.h"
#include <bmkernel_api.h>
#include <onnc/Support/Debug.h>

namespace onnc {
namespace BM188X {

TGRelu::TGRelu(const ::onnx::Node &pNode,
               const LayerCalibrationParameter &pLayerCtable)
    : ComputeOperand2(pNode, "Relu"), m_negativeSlope(0),
      m_LayerCtable(pLayerCtable)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = inDim[2].dim;
    m_W = inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = 1;
    m_C = 1;
    m_H = inDim[0].dim;
    m_W = inDim[1].dim;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }
}

void TGRelu::print(OStream &pOS) const
{
  pOS << *m_MemOperands[1] << " = ReLU <negativeSlope:" << m_negativeSlope
      << ", N:" << m_N << ", C:" << m_C << ", H:" << m_H << ", W:" << m_W
      << "> (" << *m_MemOperands[1] << ")\n";
}

void TGRelu::emit() const
{
  DEBUG(print(dbgs()));

  bmnet::bmnet_relu_fixed_forward_bmkernel(
      *bm1880_kernel::getInstance().m_Ctx,
      m_MemOperands[0]->addr, // input_gaddr
      m_MemOperands[1]->addr, // output_gaddr
      m_negativeSlope,        // negative_slope
      m_N,                    // input_n
      m_C,                    // input_c
      m_H,                    // input_h
      m_W                     // input_w
  );
}

} // namespace BM188X
} // namespace onnc
