//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CodeEmitVisitor.h"
#include "Compute/AveragePool.h"
#include "TGBackend.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "bm188x_codeemit"

using namespace onnc;
using namespace onnc::BM188X;

#define USE_NEW_CE 0

//===----------------------------------------------------------------------===//
// CodeEmitVisitor
//===----------------------------------------------------------------------===//
void BM188X::CodeEmitVisitor::visit(const BM188X::AveragePool& pOperator)
{
  auto *ifmap = m_TGBackend->getMemOpndByValue(pOperator.getInput(0));
  auto *ofmap = m_TGBackend->getMemOpndByValue(pOperator.getOutput(0));

  const onnc::Tensor* inTensor = pOperator.getInput(0);
  int n = inTensor->dimension(0),
      c = inTensor->dimension(1),
      h = inTensor->dimension(2),
      w = inTensor->dimension(3);

  int kh = pOperator.getKernelShape().vector()[0],
      kw = pOperator.getKernelShape().vector()[1];

  int padt = pOperator.getPads().vector()[0],
      padl = pOperator.getPads().vector()[1],
      padb = pOperator.getPads().vector()[2],
      padr = pOperator.getPads().vector()[3];

  int strh = pOperator.getStrides().vector()[0],
      strw = pOperator.getStrides().vector()[1];

  int enRelu = pOperator.getEnableRelu(),
      rsWidth = pOperator.getRShiftWidth(),
      xq = pOperator.getThresholdXQuantized();

  DEBUG(dbgs()
    << "BM188X::AveragePool" << "\n"
    << "  " << ifmap->start() << " " << ofmap->start()
    << " " << n << " " << c << " " << h << " " << w << " "
    << kh << " " << kw << " "
    << padt << " " << padb << " " << padl << " " << padr << " "
    << strh << " " << strw << " "
    << enRelu << " " << rsWidth << " " << xq << " "
    << "\n");

#if USE_NEW_CE
  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap_gaddr
      m_MemOperands[1]->m_Addr,        // ofmap_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // index_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // o_findex_gaddr
      n, c, h, w, kh, kw, padt, padb, padl, padr, strh, strw,
      1,                               // is_avg_pooling
      0.0f,                            // avg_const
      enRelu,                          // do_relu
      rsWidth,                         // right_shift_width
      &xq,                             // threshold_x_quantized
      0                                // ceil_mode
  );
#endif
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Concat& pOperator)
{
  /**
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
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Conv& pOperator)
{
  /**
  float activation_arg[1] = { 0.0f };

  uint64_t biasAddr = m_DoBias ? m_MemOperands[m_BiasIdx]->m_Addr
                               : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_addr = m_DoScale ? m_MemOperands[m_ScaleIdx]->m_Addr
                                  : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_bias_addr = m_DoScaleBias
                                 ? m_MemOperands[m_ScaleBiasIdx]->m_Addr
                                 : bmnet::bmnet_asm::GADDR_INVALID;
  bmnet::bmnet_asm::bmnet_conv_parallel_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap
      m_MemOperands[2]->m_Addr,        // ofmap
      m_MemOperands[1]->m_Addr,        // weight
      biasAddr,                        // bias
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_mean,
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_variance,
      scale_addr,                      // ga_scale,
      scale_bias_addr,                 // ga_scale_bias,
      m_InN, m_InC, m_InH, m_InW, m_Groups, m_OutC, m_KH, m_KW, m_DilationH,
      m_DilationW, m_PadH, m_PadW, m_StrideH, m_StrideW, false, // result_add
      m_DoBias,                                                 // do_bias,
      0,                                                        // do_bn,
      m_DoScale, m_DoScaleBias, pm::match(m_pNode, pm::mTrueAttr("do_relu")),
      0,                  // bn_scale,
      0,                  // bn_eps,
      0,                  // activation_method,
      activation_arg,     // activation_arg[],
      0,                  // activation_ga_slope,
      0,                  // activation_channel_shared
      0,                  // activation_gt_rshift
      0,                  // activation_gt_rshift
      0,                  // activation_le_scale
      0,                  // activation_le_rshift
      m_RShiftWidth,      // right_shift_width
      0,                  // bn_right_shift_width
      m_ScaleRShiftWidth, // scale_right_shift_width
      0                   // use_winograd
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Gemm& pOperator)
{
  /**
  int do_activation = m_EnableRelu;
  int activation_method = RELU;

  bmnet::bmnet_asm::bmnet_fc_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // input_data_gaddr
      m_MemOperands[1]->m_Addr,        // weight_data_gaddr
      m_MemOperands[2]->m_Addr,        // bias_data_gaddr
      m_MemOperands[3]->m_Addr,        // output_data_gaddr
      m_InRowNum,                      // input_row_num
      m_InColNum,                      // input_col_num
      m_OutColNum,                     // weight_col_num
      m_HaveBias,                      // have_bias
      do_activation,                   // do_activation
      activation_method,               // activation_method
      bmnet::bmnet_asm::GADDR_INVALID, // activation_ga_slope
      0,                               // activation_channel_shared
      0,                               // activation_gt_scale
      0,                               // activation_gt_rshift
      0,                               // activation_le_scale
      0,                               // activation_le_rshift
      m_WeightTp,                      // weight_transpose
      0,                               // left_shift_width //TODO
      m_RShiftWidth                    // right_shift_width
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::GlobalAveragePool& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap_gaddr
      m_MemOperands[1]->m_Addr,        // ofmap_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // index_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_H, m_W, 0, 0, 0, 0, 1, 1,
      1,                      // is_avg_pooling
      0.0f,                   // avg_const
      m_EnableRelu,           // do_relu
      m_RShiftWidth,          // right_shift_width
      &m_ThresholdXQuantized, // threshold_x_quantized
      0                       // ceil_mode
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::LRN& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_lrn_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input
      m_MemOperands[3]->m_Addr, // output
      m_MemOperands[1]->m_Addr, // sqr_lut
      m_MemOperands[2]->m_Addr, // power_lut,
      m_N, m_C, m_H, m_W, m_LocalSize, m_SumRightShiftWidth,
      m_LrnRightShiftWidth, m_ThresholdXQuantized);
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::LeakyRelu& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_leakyrelu_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // output_gaddr
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W,                      // input_w
      m_GTRShiftWidth,          // GT_right_shift_width
      m_LERShiftWidth,          // LE_right_shift_width
      m_GTScale,                // GT_scale
      m_LEScale);               // LE_scale
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Load& pOperator)
{
  /**
  // Calculate the address after Global Memory Allocation Pass
  uint64_t gaddr = m_SrcGOffset + m_MemOperands[0]->m_Addr;

  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  // TODO(arcbbb): only support 4d tensor for the moment
  bmnet::bmnet_asm::bmnet_tl_load_stride_bmkernel(
      gaddr,         // Src global addr
      m_DstLAddr,    // Dest local addr
      m_LocalN,      // Local N
      m_LocalC,      // Local C
      m_LocalH,      // Local H
      m_LocalW,      // Local W
      m_GlobalC,     // Global C
      m_GlobalH,     // Global H
      m_GlobalW,     // Global W
      m_DoTranspose, // Do Transpose
      m_IsAligned,   // Check alignment
      m_IsNeuron     // MemSpace: Neuron or Weight
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::MaxPool& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap_gaddr
      m_MemOperands[1]->m_Addr,        // ofmap_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // index_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_KH, m_KW, m_PadT, m_PadB, m_PadL, m_PadR, m_StrideH,
      m_StrideW,
      0,                      // is_avg_pooling
      0.0f,                   // avg_const
      0,                      // do_relu
      m_RShiftWidth,          // right_shift_width
      &m_ThresholdXQuantized, // threshold_x_quantized
      0                       // ceil_mode
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::PRelu& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_prelu_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // slope_gaddr
      m_MemOperands[2]->m_Addr, // output_gaddr
      m_ChannelShared,          // channel_shared
      m_Slope,                  // slope
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W,                      // input_w
      m_GTScale,                // GT_scale
      m_GTRShiftWidth,          // GT_right_shift_width
      m_LERShiftWidth);         // LE_right_shift_width
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Pool& pOperator)
{
  /**
  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  bmnet::bmnet_asm::bmnet_tl_pooling_forward_bmkernel(
      m_IFmapAddr, // ifmap
      m_OFmapAddr, // ofmap
      m_InN, m_InC, m_InH, m_InW, m_OutN, m_OutC, m_OutH, m_OutW, m_KH, m_KW,
      m_StrideH, m_StrideW,                          // stride
      m_PadHTop, m_PadHBot, m_PadWLeft, m_PadWRight, // padding
      m_IsAvgPooling,                                // is_avg_pooling
      m_RShiftWidth, m_ThresholdXQuantized);
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Relu& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_relu_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // output_gaddr
      m_NegativeSlope,          // negative_slope
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W                       // input_w
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Scale& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_scale_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input
      m_MemOperands[1]->m_Addr, // scale
      m_MemOperands[2]->m_Addr, // bias
      m_MemOperands[3]->m_Addr, // outpur,
      m_N, m_C, m_H, m_W, m_MScaleDim, m_MInnerDim, m_RShiftWidth);
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Store& pOperator)
{
  /**
  // Calculate the address after Global Memory Allocation Pass
  uint64_t gaddr = m_DstGOffset + m_MemOperands[0]->m_Addr;

  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  // TODO(arcbbb): only support 4d tensor for the moment
  bmnet::bmnet_asm::bmnet_tl_store_stride_bmkernel(
      gaddr,         // Dest global addr
      m_SrcLAddr,    // Src  local addr
      m_LocalN,      // Local N
      m_LocalC,      // Local C
      m_LocalH,      // Local H
      m_LocalW,      // Local W
      m_GlobalC,     // Global C
      m_GlobalH,     // Global H
      m_GlobalW,     // Global W
      m_DoTranspose, // Do Transpose
      m_IsAligned,   // Check alignment
      m_IsNeuron     // MemSpace: Neuron or Weight
  );
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Sum& pOperator)
{
  /**
  int in_size = m_MemOperands.size() - 1;
  uint64_t *input = new uint64_t[in_size];
  for (int i = 0; i < in_size; ++i)
    input[i] = m_MemOperands[i]->m_Addr;

  bmnet::bmnet_asm::bmnet_eltwise_fixed_forward_bmkernel(
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
  **/
}

void BM188X::CodeEmitVisitor::visit(const BM188X::Transpose& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_permute_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr, m_N, m_C, m_H, m_W,
      m_OutputShape[0], m_OutputShape[1], m_OutputShape[2], m_OutputShape[3],
      m_Order[0], m_Order[1], m_Order[2], m_Order[3], m_NeedPermute);
  **/
}

void BM188X::CodeEmitVisitor::visit(const onnc::Upsample& pOperator)
{
  /**
  bmnet::bmnet_asm::bmnet_upsample_fixed_bmkernel(
      m_MemOperands[0]->m_Addr, // ifmap_gaddr
      m_MemOperands[1]->m_Addr, // ofmap_gaddr
      m_N, m_C, m_H, m_W, m_Scale);
  **/
}
