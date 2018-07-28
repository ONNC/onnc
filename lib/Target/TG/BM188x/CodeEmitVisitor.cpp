//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CodeEmitVisitor.h"
#include "Compute/AveragePool.h"
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// CodeEmitVisitor
//===----------------------------------------------------------------------===//
void BM188X::CodeEmitVisitor::visit(const BM188X::AveragePool& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap_gaddr
      m_MemOperands[1]->m_Addr,        // ofmap_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // index_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_KH, m_KW, m_PadT, m_PadB, m_PadL, m_PadR, m_StrideH,
      m_StrideW,
      1,                      // is_avg_pooling
      0.0f,                   // avg_const
      m_EnableRelu,           // do_relu
      m_RShiftWidth,          // right_shift_width
      &m_ThresholdXQuantized, // threshold_x_quantized
      0                       // ceil_mode
  );
  **/
}
