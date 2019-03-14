//===- CodeEmitVisitor.cpp ------------------------------------------------===//
// CodeEmitVisitor for NVDLA backend
//
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CodeEmitVisitor.h"

#include "Compute/NvDlaConvRelu.h"
#include "Compute/NvDlaConvReluMaxPool.h"
#include "Compute/NvDlaGemmRelu.h"
#include "Compute/NvDlaMaxPool.h"
#include "fp16.h"

#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;
using namespace onnc::nvdla;

namespace onnc {
namespace internal {
bool isFirstOperator(const Conv& conv) {
  const Tensor& input = *conv.getInput(0);

  const auto* source = dynamic_cast<const InputOperator*>(input.getDefine());
  return source != nullptr;
}
} // namespace internal
} // namespace onnc
void CodeEmitVisitor::visit(const Initializer& pInitializer)
{
  pInitializer.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const InputOperator& pInputOperator)
{
  pInputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const OutputOperator& pOutputOperator)
{
  pOutputOperator.print(errs());
  errs() << "\n";
}

void CodeEmitVisitor::visit(const Conv& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);

  const bool is_first_op     = internal::isFirstOperator(pOp);
  const bool is_image_mode   = (is_first_op && (3 == input_X_dims[1] || 4 == input_X_dims[1]));
  const bool should_pad_zero = (is_image_mode && input_X_dims[1] < 4);
#if HAS_IMAGE_MODE
  if (is_image_mode) {
    input_X_dims[1] = 4; // Hardware requires image to be 4 channels
  }
#endif
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);
  printf("DBG0: X banks %d\n", X_cube.banks);

  const Tensor* input_W_t = pOp.getInput(1);
  // void *input_W = m_ATable[input_W_t];
  int32_t input_W_ndim    = input_W_t->getNumOfDimensions();
  int32_t input_W_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_W_ndim; ++i)
    input_W_dims[i] = input_W_t->dimension(i);
#if HAS_IMAGE_MODE
  if (is_image_mode) {
    input_W_dims[1] = 4; // Hardware requires image to be 4 channels
  }
#endif

  const Tensor* input_B_t    = NULL;
  void*         input_B      = NULL;
  int32_t       input_B_ndim = 0;
  if (pOp.getNumOfInputs() > 2) {
    input_B_t = pOp.getInput(2);
    // input_B = m_ATable[input_B_t];
    input_B_ndim = input_B_t->getNumOfDimensions();
  }
  int32_t input_B_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_B_ndim; ++i)
    input_B_dims[i] = input_B_t->dimension(i);
  // Prepare output

  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
  ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);
  printf("DBG0: Y banks %d\n", Y_cube.banks);

  // Prepare attributes
  const char* auto_pad            = pOp.getAutoPad().value().c_str();
  int32_t     number_of_dilations = pOp.getDilations().vector().size();
  int32_t     dilations[2]        = {1, 1};
  for (int i = 0; i < number_of_dilations; ++i)
    dilations[i] = pOp.getDilations().at(i);
  int32_t group                  = pOp.getGroup().value();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[2]        = {1, 1};
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pads[4]        = {0, 0, 0, 0};
  for (int i = 0; i < number_of_pads; ++i)
    pads[i] = pOp.getPads().at(i);
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[2]        = {1, 1};
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  NVDLA_DBG("d(%d, %d), k(%d, %d) p(%d, %d, %d, %d) s(%d, %d)", dilations[0], dilations[1], kernel_shape[0],
            kernel_shape[1], pads[0], pads[1], pads[2], pads[3], strides[0], strides[1]);

  if (group > 1) {
    input_X_dims[1] /= group;
    input_W_dims[0] /= group;
    input_B_dims[0] /= group;
    output_Y_dims[1] /= group;
  }

  NvDlaCubeInfo fcube_group((is_image_mode) ? NVDLA_CUBE_IMAGE : NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1],
                            input_X_dims[2], input_X_dims[3]);
  NvDlaCubeInfo winfo(NVDLA_CUBE_WEIGHT, input_W_dims[0], input_W_dims[1], input_W_dims[2], input_W_dims[3]);
  printf("DBG0: f banks %d, w banks %d\n", fcube_group.banks, winfo.banks);
  NVDLA_DBG("conv(%d) f(%d %d %d %d eps:%d banks:%d) w(%d %d %d %d banks:%d) b(%d %d %d %d) y(%d %d %d %d)\n", group,
            input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3], fcube_group.eps, fcube_group.banks,
            input_W_dims[0], input_W_dims[1], input_W_dims[2], input_W_dims[3], winfo.banks, input_B_dims[0],
            input_B_dims[1], input_B_dims[2], input_B_dims[3], output_Y_dims[0], output_Y_dims[1], output_Y_dims[2],
            output_Y_dims[3]);

  // TODO: pads!!!
  for (int g = 0; g < group; g++) {
    // Weight Memory allocation, repacking by groups
    int W_mid  = packWeight(input_W_t, input_W_dims, g, should_pad_zero);
    int W_addr = issueDlaAddr(W_mid, winfo, 1, 0, 0);
    int B_mid  = -1;
    int B_addr = -1;

    NvDlaCubeInfo B_info(NVDLA_CUBE_FEATURE, input_B_dims[0], input_B_dims[1], input_B_dims[2], input_B_dims[3]);
    printf("DBG0: B banks %d\n", B_info.banks);
    ILoadable::MemoryListEntry B_mle;
    if (pOp.getNumOfInputs() > 2) {
      // TODO: packed with FEATURE layout
      B_mid  = packBias(input_B_t, input_B_dims, g);
      B_mle  = m_pMeta.m_MemoryListEntries[B_mid];
      B_addr = issueDlaAddr(B_mid, B_info, 1, 0, 0);
    }

    // X, B, Y try to use offset for split & group
    int pad_size              = pads[0] + pads[1];
    int kernel_size_minus_one = (input_W_dims[2] - 1);
    int max_conv_H            = input_X_dims[2] - kernel_size_minus_one;
    if (fcube_group.banks + winfo.banks > CBUF_BANK_NUM) {
      if (fcube_group.banks + winfo.getReducedBanks() <= CBUF_BANK_NUM) {
        printf("DBG1: f banks %d, w banks %d\n", fcube_group.banks, winfo.banks);
        winfo.reduceBanks();
        printf("DBG1: f banks %d, w banks %d\n", fcube_group.banks, winfo.banks);
      } else {
        // split
        int restBanks = CBUF_BANK_NUM - winfo.banks;
        if (restBanks > 0) {
          max_conv_H = (CBUF_BANK_DEPTH * restBanks) / fcube_group.eps;
          NVDLA_DBG("max_conv_H: %d\n", max_conv_H);
        } else {
          // TODO, return critical error
        }
      }
    }
    // int total_h = input_X_dims[2] - (input_W_dims[2]-1);
    // int split_H = input_X_dims[2] - (input_W_dims[2]-1);
    int total_h = input_X_dims[2] - kernel_size_minus_one;
    int split_H = max_conv_H;
    printf("DBG4: split_H %d, total_h %d, max_conv_H %d, kernel_size_minus_one %d\n", split_H, total_h, max_conv_H, kernel_size_minus_one);
    for (int h = 0, dst_h = 0; h < total_h; h += split_H) {
      int pad_top = (h == 0) ? pads[0] : 0;
      if (max_conv_H != total_h) {
        split_H = (((max_conv_H - kernel_size_minus_one + pad_top) / strides[0]) * strides[0]) - pad_top;
      }
      int op_h = h + split_H >= total_h ? total_h - h : split_H;
      printf("DBG5: split_H %d, op_h %d, pad_top %d, op_h+kernel_size_minus_one %d\n", split_H, op_h, pad_top,
             op_h + kernel_size_minus_one);
      int pad_bottom = h + split_H >= total_h ? pads[1] : 0;
      int output_h   = ((op_h + pad_top + pad_bottom) + (strides[0] - 1)) / strides[0];

      NVDLA_DBG("conv op_h[%d] output_h[%d]\n", op_h + kernel_size_minus_one, output_h);
      NvDlaCubeInfo finfo((is_image_mode) ? NVDLA_CUBE_IMAGE : NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1],
                          op_h + kernel_size_minus_one, input_X_dims[3]);
      NvDlaCubeInfo oinfo(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_h, output_Y_dims[3]);
      printf("DBG0: f banks %d, o banks %d\n", finfo.banks, oinfo.banks);

      NvDlaDlaOperation* conv_op = new NvDlaDlaOperation();
      NvDlaDlaOperation* add_op  = NULL;
      conv_op->op_dep.op_type    = DLA_OP_CONV;

      struct dla_conv_op_desc* conv_desc = (struct dla_conv_op_desc*)(&(conv_op->op_desc));
      conv_desc->conv_mode               = CONV_MODE_DIRECT;
      conv_desc->data_reuse              = 0;
      conv_desc->weight_reuse            = (h == 0 || winfo.reduced) ? 0 : 1;
      conv_desc->skip_data_rls           = 0;
      conv_desc->skip_weight_rls         = (!winfo.reduced && (h + split_H < total_h)) ? 1 : 0;
      conv_desc->entry_per_slice         = finfo.eps;
      conv_desc->data_format             = FORMAT_FEATURE;
#if HAS_IMAGE_MODE
      // Golden image_mode loadable has data_format 15 = R8G8B8A8.
      // FIXME: for now, always use data_format 15 for image mode.
      // FIXME: for now, use image mode if 3-channel, reardless of whether it's the first model layer.
      if (finfo.dim_c == 3 || finfo.dim_c == 4) {
        conv_desc->data_format = 15;
      }
#endif
      conv_desc->pixel_mapping = 0;
      conv_desc->fetch_grain   = 1;
      conv_desc->batch         = 1;
      conv_desc->weight_format = WEIGHT_FORMAT_UNCOMPRESSED;
      conv_desc->data_bank     = finfo.banks;
      conv_desc->weight_bank   = winfo.banks;
      printf("DBG2: f banks %d, w banks %d\n", fcube_group.banks, winfo.banks);
      conv_desc->batch_stride   = 0;
      conv_desc->post_extension = 0;
      conv_desc->pixel_override = 0;
#if DLA_NV_SMALL
      conv_desc->release = 1; // FIXME: we found its value is always 1 in nv_small_alexnet.nvdla
#else
      conv_desc->release = op_h + kernel_size_minus_one;
#endif
      conv_desc->input_width_csc    = finfo.dim_w;
      conv_desc->input_height_csc   = finfo.dim_h;
      conv_desc->input_channel_csc  = finfo.dim_c;
      conv_desc->kernel_channel_csc = winfo.dim_c;
      conv_desc->kernel_width_csc   = winfo.dim_w;
      conv_desc->kernel_height_csc  = winfo.dim_h;
#if HAS_IMAGE_MODE
      // FIXME: for now, use image mode if 3-channel, reardless of whether it's the first model layer.
      if (finfo.dim_c == 3 || finfo.dim_c == 4) {
        conv_desc->kernel_channel_csc = winfo.dim_w * winfo.dim_c;
        conv_desc->kernel_width_csc   = 1;
        conv_desc->kernel_height_csc  = winfo.dim_h;
      }
#endif
      conv_desc->input_width_cmac  = output_Y_dims[3];
      conv_desc->input_height_cmac = output_h;
      conv_desc->bytes_per_kernel  = winfo.dim_c * winfo.dim_h * winfo.dim_w * ELEMENT_SIZE;
      conv_desc->mean_ry           = 0;
      conv_desc->mean_gu           = 0;
      conv_desc->mean_bv           = 0;
      conv_desc->mean_ax           = 0;
      conv_desc->mean_format       = 0;
      conv_desc->conv_stride_x     = strides[1];
      conv_desc->conv_stride_y     = strides[0];
      conv_desc->pad_x_left        = pads[2];
      conv_desc->pad_x_right       = pads[3];
      conv_desc->pad_y_top         = pad_top;
      conv_desc->pad_y_bottom      = pad_bottom;
      conv_desc->dilation_x        = dilations[1];
      conv_desc->dilation_y        = dilations[0];
      conv_desc->pra_truncate      = 0;
      conv_desc->in_precision      = DLA_PRECISION; // INT8 vs INT16 vs FP16
      conv_desc->out_precision     = DLA_PRECISION; // INT8 vs INT16 vs FP16
      conv_desc->out_cvt.scale     = 1;
      conv_desc->out_cvt.enable    = 1;
      conv_desc->pad_val           = 0;

      ILoadable::MemoryListEntry    W_mle     = m_pMeta.m_MemoryListEntries[W_mid];
      struct dla_conv_surface_desc* conv_surf = (struct dla_conv_surface_desc*)(&(conv_op->op_surf));
      conv_surf->weight_data.type             = DLA_MEM_MC;
      conv_surf->weight_data.address          = W_addr;
      conv_surf->weight_data.size             = W_mle.size;
      conv_surf->weight_data.width            = winfo.dim_w;
      conv_surf->weight_data.height           = winfo.dim_h;
      conv_surf->weight_data.channel          = winfo.dim_c;
      conv_surf->weight_data.line_stride      = 0;
      conv_surf->weight_data.surf_stride      = 0;
      conv_surf->weight_data.plane_stride     = 0;

      conv_surf->wmb_data.type    = DLA_MEM_HW;
      conv_surf->wmb_data.address = -1;

      conv_surf->wgs_data.type    = DLA_MEM_HW;
      conv_surf->wgs_data.address = -1;

      int num_atomc                    = (finfo.dim_c + FEATURE_ATOM_CUBE_SIZE - 1) / FEATURE_ATOM_CUBE_SIZE;
      conv_surf->src_data.type         = DLA_MEM_MC;
      conv_surf->src_data.address      = issueDlaAddr(X_mid, X_cube, group, g, h * num_atomc);
      conv_surf->src_data.size         = finfo.size;
      conv_surf->src_data.width        = finfo.dim_w;
      conv_surf->src_data.height       = finfo.dim_h;
      conv_surf->src_data.channel      = finfo.dim_c;
      conv_surf->src_data.line_stride  = finfo.stride_line;
      conv_surf->src_data.surf_stride  = finfo.stride_surface;
      conv_surf->src_data.plane_stride = finfo.stride_plane;

      num_atomc                        = (oinfo.dim_c + FEATURE_ATOM_CUBE_SIZE - 1) / FEATURE_ATOM_CUBE_SIZE;
      conv_surf->dst_data.type         = DLA_MEM_MC; // FIXME: conv has no write-DMA. dst_data.type should also be DLA_MEM_HW.
      conv_surf->dst_data.address      = issueDlaAddr(Y_mid, Y_cube, group, g, dst_h * num_atomc);
      conv_surf->dst_data.size         = oinfo.size;
      conv_surf->dst_data.width        = oinfo.dim_w;
      conv_surf->dst_data.height       = oinfo.dim_h;
      conv_surf->dst_data.channel      = oinfo.dim_c;
      conv_surf->dst_data.line_stride  = Y_cube.stride_line;
      conv_surf->dst_data.surf_stride  = Y_cube.stride_surface;
      conv_surf->dst_data.plane_stride = Y_cube.stride_plane;

#if 0 // HAS_IMAGE_MODE
      // FIXME: for now, use image mode if 3-channel, reardless of whether it's the first model layer.
      if (finfo.dim_c == 3 || finfo.dim_c == 4) {
        //conv_surf->weight_data.size = winfo.dim_w * winfo.dim_h * winfo.dim_c * winfo.dim_n;
        conv_surf->src_data.size = finfo.dim_w * finfo.dim_h * finfo.dim_c;
        conv_surf->src_data.line_stride = (finfo.dim_w * finfo.dim_c + (FEATURE_ATOM_CUBE_SIZE-1)) / FEATURE_ATOM_CUBE_SIZE * FEATURE_ATOM_CUBE_SIZE;
        //conv_surf->src_data.surf_stride = 0;
        //conv_surf->dst_data.size = 0;
      }
#endif

      // Bias Add
      if (pOp.getNumOfInputs() > 2) {
        add_op                 = new NvDlaDlaOperation();
        add_op->op_dep.op_type = DLA_OP_SDP;

        struct dla_sdp_op_desc* add_desc     = (struct dla_sdp_op_desc*)(&(add_op->op_desc));
        add_desc->src_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
        add_desc->dst_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
        add_desc->lut_index                  = -1;
        add_desc->conv_mode                  = 0;
        add_desc->out_cvt.scale              = 1;
        add_desc->out_cvt.truncate           = 0;
        add_desc->out_cvt.enable             = 1;
        add_desc->out_cvt.offset             = 0;
        add_desc->conv_mode                  = CONV_MODE_DIRECT;
        add_desc->batch_num                  = 1;
        add_desc->batch_stride               = 0;
        add_desc->x1_op.enable               = 1;
        add_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
        add_desc->x1_op.type                 = SDP_OP_ADD;
        add_desc->x1_op.mode                 = SDP_OP_PER_KERNEL;
        add_desc->x1_op.act                  = ACTIVATION_NONE;
        add_desc->x1_op.shift_value          = 0;
        add_desc->x1_op.truncate             = 0;
        add_desc->x1_op.precision            = DLA_PRECISION; // INT8 vs INT16 vs FP16
        add_desc->x1_op.alu_operand          = 0;
        add_desc->x1_op.mul_operand          = 0; // 1; //FIXME: why 1? Should be 0 when x1_op.mode == SDP_OP_PER_KERNEL
        add_desc->x1_op.cvt.alu_cvt.scale    = 0;
        add_desc->x1_op.cvt.alu_cvt.truncate = 0;
        add_desc->x1_op.cvt.alu_cvt.enable   = 0;
        add_desc->x1_op.cvt.alu_cvt.offset   = 0;
        add_desc->x1_op.cvt.mul_cvt.scale    = 0;
        add_desc->x1_op.cvt.mul_cvt.truncate = 0;
        add_desc->x1_op.cvt.mul_cvt.enable   = 0;
        add_desc->x1_op.cvt.mul_cvt.offset   = 0;
        add_desc->x2_op.alu_type             = SDP_ALU_OP_SUM; // FIXME: since x2_op.enable is 0, I don't think it needs any
                                                   // paticular value. This value is from nv_small_alexnet.nvdla.
        add_desc->y_op.alu_type = SDP_ALU_OP_SUM;

        struct dla_sdp_surface_desc* add_surf = (struct dla_sdp_surface_desc*)(&(add_op->op_surf));
        add_surf->src_data.type               = DLA_MEM_HW;
        add_surf->src_data.address            = -1;
#if DLA_NV_SMALL
        add_surf->src_data.size = 0; // FIXME: in nv_small_alexnet.nvdla it's 0. In KMD, I don't see where to use this
                                     // value. Seems this value does not impact anything in KMD.
#else
        add_surf->src_data.size         = conv_surf->dst_data.size;
#endif
        add_surf->src_data.width   = conv_surf->dst_data.width;
        add_surf->src_data.height  = conv_surf->dst_data.height;
        add_surf->src_data.channel = conv_surf->dst_data.channel;
#if DLA_NV_SMALL
        add_surf->src_data.line_stride  = 0; // oinfo.stride_line; //FIXME?
        add_surf->src_data.surf_stride  = 0; // oinfo.stride_surface;
        add_surf->src_data.plane_stride = 0; // oinfo.stride_plane;
#else
        add_surf->src_data.line_stride  = oinfo.stride_line;
        add_surf->src_data.surf_stride  = oinfo.stride_surface;
        add_surf->src_data.plane_stride = oinfo.stride_plane;
#endif
        add_surf->x1_data.type         = DLA_MEM_MC;
        add_surf->x1_data.address      = B_addr;
        add_surf->x1_data.size         = B_mle.size;
        add_surf->x1_data.width        = input_B_dims[3];
        add_surf->x1_data.height       = input_B_dims[2];
        add_surf->x1_data.channel      = input_B_dims[0];
        add_surf->x1_data.line_stride  = B_info.stride_line;
        add_surf->x1_data.surf_stride  = B_info.stride_surface;
        add_surf->x1_data.plane_stride = B_info.stride_plane;
        NVDLA_DBG("B sl[%d] ss[%d] sp[%d]\n", B_info.stride_line, B_info.stride_surface, B_info.stride_plane);

        add_surf->dst_data.type         = DLA_MEM_MC;
        add_surf->dst_data.address      = conv_surf->dst_data.address;
        add_surf->dst_data.size         = conv_surf->dst_data.size;
        add_surf->dst_data.width        = conv_surf->dst_data.width;
        add_surf->dst_data.height       = conv_surf->dst_data.height;
        add_surf->dst_data.channel      = conv_surf->dst_data.channel;
        add_surf->dst_data.line_stride  = conv_surf->dst_data.line_stride;
        add_surf->dst_data.surf_stride  = conv_surf->dst_data.surf_stride;
        add_surf->dst_data.plane_stride = conv_surf->dst_data.plane_stride;
#if HAS_IMAGE_MODE
        // FIXME: for now, use image mode if 3-channel, reardless of whether it's the first model layer.
        if (finfo.dim_c == 3 || finfo.dim_c == 4) {
          add_surf->dst_data.size = oinfo.size; // the regular conv_surf->dst_data.size
        }
#endif

        conv_surf->dst_data.type    = DLA_MEM_HW;
        conv_surf->dst_data.address = -1;
#if DLA_NV_SMALL
        conv_surf->dst_data.size = 0; // FIXME: really needs 0???
#endif
        // conv_surf->dst_data.line_stride = 0;
        // conv_surf->dst_data.surf_stride = 0;
        // conv_surf->dst_data.plane_stride = 0;
      }
      NvDlaDlaOperation* prev_op = (h == 0) ? m_pMeta.m_pPrevOp : NULL;
      issueDlaOp(conv_op, add_op, prev_op);

      // generate CONV Operation
      NVDLA_DBG("split-conv s:%d e:%d h:%d, dst_h:%d, output_h=%d\n", h, h + op_h, op_h + kernel_size_minus_one, dst_h, output_h);
      dst_h += output_h;
    }
  }

  printf("DBG3: f banks %d, w banks %d\n", fcube_group.banks, winfo.banks);
}

void CodeEmitVisitor::visit(const Reshape& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  // Prepare input
  const Tensor* input_data_t = pOp.getInput(0);
  // void *input_data = m_ATable[input_data_t];
  int32_t input_data_ndim    = input_data_t->getNumOfDimensions();
  int32_t input_data_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_data_ndim; ++i)
    input_data_dims[i] = input_data_t->dimension(i);
  const Tensor*              input_shape_t = pOp.getInput(1);
  int                        data_idx      = m_pMeta.m_MemIdxTable[(Tensor*)input_data_t];
  ILoadable::MemoryListEntry data_mle      = m_pMeta.m_MemoryListEntries[data_idx];

  // void *input_shape = m_ATable[input_shape_t];
  int32_t input_shape_ndim    = input_shape_t->getNumOfDimensions();
  int32_t input_shape_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_shape_ndim; ++i)
    input_shape_dims[i] = input_shape_t->dimension(i);

  // Prepare output
  // void *output_reshaped = m_ATable[output_reshaped_t];
  const Tensor* output_reshaped_t       = pOp.getOutput(0);
  int32_t       output_reshaped_ndim    = output_reshaped_t->getNumOfDimensions();
  int32_t       output_reshaped_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_reshaped_ndim; ++i)
    output_reshaped_dims[i] = output_reshaped_t->dimension(i);
  // int out_shape_idx = m_pMeta.m_MemIdxTable[(Tensor *)output_reshaped_t];
  // ILoadable::MemoryListEntry out_shape_mle = m_pMeta.m_MemoryListEntries[out_shape_idx];

  // TODO, setup remapping table
  // m_pMeta.m_ReshapeTable[output_reshaped_t] = input_data_t;
}

// Setup LookupTable
void CodeEmitVisitor::visit(const LRN& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  // Prepare input
  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare outputSNESSNES
  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
  ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  // Prepare attributes
  float   alpha = pOp.getAlpha().value();
  float   beta  = pOp.getBeta().value();
  float   bias  = pOp.getBias().value();
  int32_t size  = pOp.getSize().value();
  NVDLA_DBG("LRN(alpha:%f beta:%f bias:%f size:%d)\n", alpha, beta, bias, size);

  int lut_id = -1;
  if (alpha != 0.0001f || beta != 0.75f || bias != 1.0f) {
    // critical error
    NVDLA_DBG("LRN(alpha:%f beta:%f bias:%f size:%d) is not matched !!!!!!!!!!!!!!!!!!!!!\n", alpha, beta, bias, size);
    struct dla_lut_param* lut_param = new dla_lut_param();
    memset(lut_param, 0, sizeof(struct dla_lut_param));
    {
      float fsize = (float)size;
      float x     = 1.0f;
      for (int i = 0; i < 65; i++) {
        lut_param->linear_exp_table[i] =
          __gnu_f2h_ieee(1.0f / pow((bias + (alpha * x / fsize)), beta)); // FIXME: bad hack method.
        x *= 2.0f;
      }
    }
    lut_param->linear_only_offset.frac_bits = -128;
    lut_param->linear_only_start            = 1;
    lut_param->linear_only_end              = 1;
    lut_id                                  = m_pMeta.m_LUTList.size();
    m_pMeta.m_LUTList.push_back(lut_param);
    m_pMeta.m_NumLUTs++;
  } else {
    lut_id = 0;
    if (m_pMeta.m_NumLUTs == 0) {
      m_pMeta.m_NumLUTs++;
    }
  }

  NvDlaDlaOperation* lrn_op = new NvDlaDlaOperation();
  lrn_op->op_dep.op_type    = DLA_OP_CDP;

  struct dla_cdp_op_desc* lrn_desc = (struct dla_cdp_op_desc*)(&(lrn_op->op_desc));
  lrn_desc->in_precision           = DLA_PRECISION; // INT8 vs INT16 vs FP16
  lrn_desc->out_precision          = DLA_PRECISION; // INT8 vs INT16 vs FP16
  lrn_desc->lut_index              = lut_id;
  lrn_desc->in_cvt.scale           = 25304; // FIXME: quantization
  lrn_desc->in_cvt.truncate        = 13;    // FIXME: quantization
  lrn_desc->in_cvt.enable          = 1;
  lrn_desc->in_cvt.offset          = 0;
  lrn_desc->out_cvt.scale          = 16587; // FIXME: quantization
  lrn_desc->out_cvt.truncate = 27;          // FIXME: need a way to determine quantization
  lrn_desc->out_cvt.enable = 1;
  lrn_desc->out_cvt.offset = 0;
  lrn_desc->local_size     = size;
  lrn_desc->bypass_sqsum   = 0;
  lrn_desc->bypass_out_mul = 0;

  struct dla_cdp_surface_desc* lrn_surf = (struct dla_cdp_surface_desc*)(&(lrn_op->op_surf));
  lrn_surf->src_data.type               = DLA_MEM_MC;
  lrn_surf->src_data.address            = issueDlaAddr(X_mid, X_cube, 1, 0, 0);
  lrn_surf->src_data.size               = X_mle.size;
  lrn_surf->src_data.width              = X_cube.dim_w;
  lrn_surf->src_data.height             = X_cube.dim_h;
  lrn_surf->src_data.channel            = X_cube.dim_c;
  lrn_surf->src_data.line_stride        = X_cube.stride_line;
  lrn_surf->src_data.surf_stride        = X_cube.stride_surface;
  lrn_surf->src_data.plane_stride       = X_cube.stride_plane;

  lrn_surf->dst_data.type         = DLA_MEM_MC;
  lrn_surf->dst_data.address      = issueDlaAddr(Y_mid, Y_cube, 1, 0, 0);
  lrn_surf->dst_data.size         = Y_mle.size;
  lrn_surf->dst_data.width        = Y_cube.dim_w;
  lrn_surf->dst_data.height       = Y_cube.dim_h;
  lrn_surf->dst_data.channel      = Y_cube.dim_c;
  lrn_surf->dst_data.line_stride  = Y_cube.stride_line;
  lrn_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  lrn_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(lrn_op, NULL, m_pMeta.m_pPrevOp);
}

void CodeEmitVisitor::visit(const MaxPool& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare output
  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
  ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  const Tensor* output_Indices_t    = NULL;
  void*         output_Indices      = NULL;
  int32_t       output_Indices_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Indices_t = pOp.getOutput(1);
    // output_Indices = m_ATable[output_Indices_t];
    output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  }
  int32_t output_Indices_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Indices_ndim; ++i)
    output_Indices_dims[i] = output_Indices_t->dimension(i);

  // Prepare attributes
  // const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pad_shapes[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i)
    pad_shapes[i] = pOp.getPads().at(i);
  int32_t storage_order     = pOp.getStorageOrder().value();
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  NvDlaDlaOperation* maxpool_op = new NvDlaDlaOperation();
  maxpool_op->op_dep.op_type    = DLA_OP_PDP;

  struct dla_pdp_op_desc* maxpool_desc = (struct dla_pdp_op_desc*)(&(maxpool_op->op_desc));
  maxpool_desc->partial_in_width_first = X_cube.dim_w; // FIXME: what does this mean in hardware?
  maxpool_desc->partial_in_width_mid  = 0;
  maxpool_desc->partial_in_width_last = 0;
  maxpool_desc->partial_width_first = Y_cube.dim_w; // FIXME: what does this mean in hardware?
  maxpool_desc->partial_width_mid  = 0;
  maxpool_desc->partial_width_last = 0;
  maxpool_desc->split_num          = 1;
  maxpool_desc->pool_mode          = POOL_MODE_MAX;
  maxpool_desc->pool_width         = kernel_shape[1] - 1;
  maxpool_desc->pool_height        = kernel_shape[0] - 1;
  maxpool_desc->stride_x           = strides[1];
  maxpool_desc->stride_y           = strides[0];
  maxpool_desc->pad_top            = pad_shapes[0]; // pad_shape - H
  maxpool_desc->pad_left           = pad_shapes[1]; // pad_shape - W
  maxpool_desc->pad_bottom         = pad_shapes[2];
  maxpool_desc->pad_right          = pad_shapes[3];

  maxpool_desc->precision = DLA_PRECISION; // INT8 vs INT16 vs FP16

  struct dla_pdp_surface_desc* maxpool_surf = (struct dla_pdp_surface_desc*)(&(maxpool_op->op_surf));
  maxpool_surf->src_data.type               = DLA_MEM_MC;
  maxpool_surf->src_data.address            = issueDlaAddr(X_mid, X_cube, 1, 0, 0);
  maxpool_surf->src_data.size               = X_mle.size;
  maxpool_surf->src_data.width              = X_cube.dim_w;
  maxpool_surf->src_data.height             = X_cube.dim_h;
  maxpool_surf->src_data.channel            = X_cube.dim_c;
  maxpool_surf->src_data.line_stride        = X_cube.stride_line;
  maxpool_surf->src_data.surf_stride        = X_cube.stride_surface;
  maxpool_surf->src_data.plane_stride       = X_cube.stride_plane;

  maxpool_surf->dst_data.type         = DLA_MEM_MC;
  maxpool_surf->dst_data.address      = issueDlaAddr(Y_mid, Y_cube, 1, 0, 0);
  maxpool_surf->dst_data.size         = Y_mle.size;
  maxpool_surf->dst_data.width        = Y_cube.dim_w;
  maxpool_surf->dst_data.height       = Y_cube.dim_h;
  maxpool_surf->dst_data.channel      = Y_cube.dim_c;
  maxpool_surf->dst_data.line_stride  = Y_cube.stride_line;
  maxpool_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  maxpool_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(maxpool_op, NULL, m_pMeta.m_pPrevOp);
}

void CodeEmitVisitor::visit(const AveragePool& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare output
  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
  ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  const Tensor* output_Indices_t    = NULL;
  void*         output_Indices      = NULL;
  int32_t       output_Indices_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Indices_t = pOp.getOutput(1);
    // output_Indices = m_ATable[output_Indices_t];
    output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  }
  int32_t output_Indices_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Indices_ndim; ++i)
    output_Indices_dims[i] = output_Indices_t->dimension(i);

  // Prepare attributes
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pad_shapes[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i)
    pad_shapes[i] = pOp.getPads().at(i);
  // int32_t storage_order = pOp.getStorageOrder().value();
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  NvDlaDlaOperation* avgpool_op = new NvDlaDlaOperation();
  avgpool_op->op_dep.op_type    = DLA_OP_PDP;

  struct dla_pdp_op_desc* avgpool_desc = (struct dla_pdp_op_desc*)(&(avgpool_op->op_desc));
  avgpool_desc->partial_in_width_first = 0;
  avgpool_desc->partial_in_width_mid   = 0;
  avgpool_desc->partial_in_width_last  = 0;
  avgpool_desc->partial_width_first    = 0;
  avgpool_desc->partial_width_mid      = 0;
  avgpool_desc->partial_width_last     = 0;
  avgpool_desc->split_num              = 1;
  avgpool_desc->pool_mode              = POOL_MODE_AVG;
  avgpool_desc->pool_width             = kernel_shape[1] - 1;
  avgpool_desc->pool_height            = kernel_shape[0] - 1;
  avgpool_desc->stride_x               = strides[1];
  avgpool_desc->stride_y               = strides[0];
  avgpool_desc->pad_top                = pad_shapes[0]; // pad_shape - H
  avgpool_desc->pad_left               = pad_shapes[1]; // pad_shape - W
  avgpool_desc->pad_bottom             = pad_shapes[2];
  avgpool_desc->pad_right              = pad_shapes[3];

  avgpool_desc->precision = DLA_PRECISION; // INT8 vs INT16 vs FP16

  struct dla_pdp_surface_desc* avgpool_surf = (struct dla_pdp_surface_desc*)(&(avgpool_op->op_surf));
  avgpool_surf->src_data.type               = DLA_MEM_MC;
  avgpool_surf->src_data.address            = issueDlaAddr(X_mid, X_cube, 1, 0, 0);
  avgpool_surf->src_data.size               = X_mle.size;
  avgpool_surf->src_data.width              = X_cube.dim_w;
  avgpool_surf->src_data.height             = X_cube.dim_h;
  avgpool_surf->src_data.channel            = X_cube.dim_c;
  avgpool_surf->src_data.line_stride        = X_cube.stride_line;
  avgpool_surf->src_data.surf_stride        = X_cube.stride_surface;
  avgpool_surf->src_data.plane_stride       = X_cube.stride_plane;

  avgpool_surf->dst_data.type         = DLA_MEM_MC;
  avgpool_surf->dst_data.address      = issueDlaAddr(Y_mid, Y_cube, 1, 0, 0);
  avgpool_surf->dst_data.size         = Y_mle.size;
  avgpool_surf->dst_data.width        = Y_cube.dim_w;
  avgpool_surf->dst_data.height       = Y_cube.dim_h;
  avgpool_surf->dst_data.channel      = Y_cube.dim_c;
  avgpool_surf->dst_data.line_stride  = Y_cube.stride_line;
  avgpool_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  avgpool_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(avgpool_op, NULL, m_pMeta.m_pPrevOp);
}

void CodeEmitVisitor::visit(const Relu& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);

  int                        Y_mid;
  ILoadable::MemoryListEntry Y_mle;
  concat_meta                meta;
  if (m_pMeta.m_ConcatTable.find(output_Y_t) == m_pMeta.m_ConcatTable.end()) {
    Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
    Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  } else {
    printf("Concat Relu ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    meta  = m_pMeta.m_ConcatTable[output_Y_t];
    Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)meta.t];
    Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  }
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  NvDlaDlaOperation* relu_op = new NvDlaDlaOperation();
  relu_op->op_dep.op_type    = DLA_OP_SDP;

  struct dla_sdp_op_desc* relu_desc     = (struct dla_sdp_op_desc*)(&(relu_op->op_desc));
  relu_desc->src_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
  relu_desc->dst_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
  relu_desc->lut_index                  = -1;
  relu_desc->conv_mode                  = 0;
  relu_desc->out_cvt.scale              = 1;
  relu_desc->out_cvt.truncate           = 0;
  relu_desc->out_cvt.enable             = 1;
  relu_desc->out_cvt.offset             = 0;
  relu_desc->conv_mode                  = CONV_MODE_DIRECT;
  relu_desc->batch_num                  = 1;
  relu_desc->batch_stride               = 0;
  relu_desc->x1_op.enable               = 1;
  relu_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
  relu_desc->x1_op.type                 = SDP_OP_NONE;
  relu_desc->x1_op.mode                 = SDP_OP_PER_LAYER;
  relu_desc->x1_op.act                  = ACTIVATION_RELU;
  relu_desc->x1_op.shift_value          = 0;
  relu_desc->x1_op.truncate             = 0;
  relu_desc->x1_op.precision            = DLA_PRECISION; // INT8 vs INT16 vs FP16
  relu_desc->x1_op.alu_operand          = 0;
  relu_desc->x1_op.mul_operand          = 1;
  relu_desc->x1_op.cvt.alu_cvt.scale    = 0;
  relu_desc->x1_op.cvt.alu_cvt.truncate = 0;
  relu_desc->x1_op.cvt.alu_cvt.enable   = 0;
  relu_desc->x1_op.cvt.alu_cvt.offset   = 0;
  relu_desc->x1_op.cvt.mul_cvt.scale    = 0;
  relu_desc->x1_op.cvt.mul_cvt.truncate = 0;
  relu_desc->x1_op.cvt.mul_cvt.enable   = 0;
  relu_desc->x1_op.cvt.mul_cvt.offset   = 0;

  struct dla_sdp_surface_desc* relu_surf = (struct dla_sdp_surface_desc*)(&(relu_op->op_surf));
  relu_surf->src_data.type               = DLA_MEM_MC;
  relu_surf->src_data.address            = issueDlaAddr(X_mid, X_cube, 1, 0, 0);
  relu_surf->src_data.size               = X_mle.size;
  relu_surf->src_data.width              = X_cube.dim_w;
  relu_surf->src_data.height             = X_cube.dim_h;
  relu_surf->src_data.channel            = X_cube.dim_c;
  relu_surf->src_data.line_stride        = X_cube.stride_line;
  relu_surf->src_data.surf_stride        = X_cube.stride_surface;
  relu_surf->src_data.plane_stride       = X_cube.stride_plane;

  relu_surf->dst_data.type = DLA_MEM_MC;
  if (m_pMeta.m_ConcatTable.find(output_Y_t) == m_pMeta.m_ConcatTable.end())
    relu_surf->dst_data.address = issueDlaAddr(Y_mid, Y_cube, 1, 0, 0);
  else
    relu_surf->dst_data.address = issueDlaAddr(Y_mid, Y_cube, -1, 0, meta.ofs);
  relu_surf->dst_data.size         = Y_mle.size;
  relu_surf->dst_data.width        = Y_cube.dim_w;
  relu_surf->dst_data.height       = Y_cube.dim_h;
  relu_surf->dst_data.channel      = Y_cube.dim_c;
  relu_surf->dst_data.line_stride  = Y_cube.stride_line;
  relu_surf->dst_data.surf_stride  = Y_cube.stride_surface;
  relu_surf->dst_data.plane_stride = Y_cube.stride_plane;

  issueDlaOp(relu_op, NULL, m_pMeta.m_pPrevOp);
}

void CodeEmitVisitor::visit(const Gemm& pOp)
{
  pOp.print(errs());
  errs() << "\n";

  // Prepare attributes
  float   alpha  = pOp.getAlpha().value();
  float   beta   = pOp.getBeta().value();
  int32_t transA = pOp.getTransA().value();
  int32_t transB = pOp.getTransB().value();

  NVDLA_DBG("GEMM tA[%d] tB[%d] alpha[%f] beta[%f]\n", transA, transB, alpha, beta);
  if (transA == 0 && transB == 1 && alpha == 1.0 && beta == 1.0) {
    // Prepare input
    const Tensor* input_A_t = pOp.getInput(0);
    // void *input_A = m_ATable[input_A_t];
    int32_t input_A_ndim    = input_A_t->getNumOfDimensions();
    int32_t input_A_dims[4] = {1, 1, 1, 1};
    for (int i = 0; i < input_A_ndim; ++i)
      input_A_dims[i] = input_A_t->dimension(i);
    int                        A_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_A_t];
    ILoadable::MemoryListEntry A_mle = m_pMeta.m_MemoryListEntries[A_mid];

    const Tensor* input_B_t = pOp.getInput(1);
    // void *input_B = m_ATable[input_B_t];
    int32_t input_B_ndim    = input_B_t->getNumOfDimensions();
    int32_t input_B_dims[4] = {1, 1, 1, 1};
    for (int i = 0; i < input_B_ndim; ++i)
      input_B_dims[i] = input_B_t->dimension(i);

    const Tensor* input_C_t = pOp.getInput(2);
    // void *input_C = m_ATable[input_C_t];
    int32_t input_C_ndim    = input_C_t->getNumOfDimensions();
    int32_t input_C_dims[4] = {1, 1, 1, 1};
    for (int i = 0; i < input_C_ndim; ++i)
      input_C_dims[i] = input_C_t->dimension(i);

    // Prepare output
    const Tensor* output_Y_t = pOp.getOutput(0);
    // void *output_Y = m_ATable[output_Y_t];
    int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
    int32_t output_Y_dims[4] = {1, 1, 1, 1};
    for (int i = 0; i < output_Y_ndim; ++i)
      output_Y_dims[i] = output_Y_t->dimension(i);
    int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
    ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];

    NVDLA_DBG("GEMM A(%d %d %d %d), B(%d %d %d %d), C(%d %d %d %d) Y(%d %d %d %d)\n", input_A_dims[0], input_A_dims[1],
              input_A_dims[2], input_A_dims[3], input_B_dims[0], input_B_dims[1], input_B_dims[2], input_B_dims[3],
              input_C_dims[0], input_C_dims[1], input_C_dims[2], input_C_dims[3], output_Y_dims[0], output_Y_dims[1],
              output_Y_dims[2], output_Y_dims[3]);

    if (m_pMeta.m_ReshapeTable.find(input_B_t) != m_pMeta.m_ReshapeTable.end()) {
      input_B_t = m_pMeta.m_ReshapeTable[input_B_t];
    }

    if (m_pMeta.m_ReshapeTable.find(input_A_t) != m_pMeta.m_ReshapeTable.end()) {
      input_A_t = m_pMeta.m_ReshapeTable[input_A_t];
      // input_A_dims[4] = {1, 1, 1, 1};
      input_A_ndim = input_A_t->getNumOfDimensions();
      for (int i = 0; i < input_A_ndim; ++i)
        input_A_dims[i] = input_A_t->dimension(i);
      A_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_A_t];
      A_mle = m_pMeta.m_MemoryListEntries[A_mid];
      for (int i = 1; i < 4; i++)
        input_B_dims[i] = input_A_dims[i];
      NVDLA_DBG("RESHAPE GEMM FOUND X(%d %d %d %d) new B(%d %d %d %d) ############################\n", input_A_dims[0],
                input_A_dims[1], input_A_dims[2], input_A_dims[3], input_B_dims[0], input_B_dims[1], input_B_dims[2],
                input_B_dims[3]);
    }

    int                        B_mid = packWeight(input_B_t, input_B_dims, 0);
    ILoadable::MemoryListEntry B_mle = m_pMeta.m_MemoryListEntries[B_mid];
    int                        C_mid = packBias(input_C_t, input_C_dims, 0);
    ILoadable::MemoryListEntry C_mle = m_pMeta.m_MemoryListEntries[C_mid];

    NvDlaDlaOperation* conv_op = new NvDlaDlaOperation();
    NvDlaDlaOperation* add_op  = NULL;
    conv_op->op_dep.op_type    = DLA_OP_CONV;

    struct dla_conv_op_desc* conv_desc = (struct dla_conv_op_desc*)(&(conv_op->op_desc));
    // NvDlaCubeInfo finfo(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], op_h + (input_W_dims[2] - 1),
    // input_X_dims[3], sizeof(short));
    NvDlaCubeInfo finfo(NVDLA_CUBE_FEATURE, input_A_dims[0], input_A_dims[1], input_A_dims[2], input_A_dims[3]);
    NvDlaCubeInfo winfo(NVDLA_CUBE_WEIGHT, input_B_dims[0], input_B_dims[1], input_B_dims[2], input_B_dims[3]);
    NvDlaCubeInfo binfo(NVDLA_CUBE_FEATURE, input_C_dims[0], input_C_dims[1], input_C_dims[2], input_C_dims[3]);
    NvDlaCubeInfo oinfo(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

    if (finfo.banks + winfo.banks > CBUF_BANK_NUM) {
      if (finfo.banks + winfo.getReducedBanks() <= CBUF_BANK_NUM) {
        winfo.reduceBanks();
      } else {
// split
#if 0
        int restBanks = 16 - winfo.banks;
        if(restBanks > 0){
          max_conv_H = (256 * restBanks) / fcube_group.eps;
          NVDLA_DBG ("max_conv_H: %d\n", max_conv_H);
        } else {
          // TODO, return critical error
        }
#endif
        // TODO, return critical error
      }
    }

    conv_desc->conv_mode       = CONV_MODE_DIRECT;
    conv_desc->data_reuse      = 0;
    conv_desc->weight_reuse    = 0;
    conv_desc->weight_reuse    = 0;
    conv_desc->skip_data_rls   = 0;
    conv_desc->skip_weight_rls = 0;
    conv_desc->entry_per_slice = finfo.eps;
    conv_desc->data_format     = FORMAT_FEATURE;
    conv_desc->pixel_mapping   = 0;
    conv_desc->fetch_grain     = 1;
    conv_desc->batch           = 1;
    conv_desc->weight_format   = WEIGHT_FORMAT_UNCOMPRESSED;
    conv_desc->data_bank       = finfo.banks;
    conv_desc->weight_bank     = winfo.banks;
    conv_desc->batch_stride    = 0;
    conv_desc->post_extension  = 0;
    conv_desc->pixel_override  = 0;
#if DLA_NV_SMALL
    conv_desc->release = 1; // FIXME: we found its value is always 1 in nv_small_alexnet.nvdla
#else
    conv_desc->release = input_A_dims[2];
#endif
    conv_desc->input_width_csc    = finfo.dim_w;
    conv_desc->input_height_csc   = finfo.dim_h;
    conv_desc->input_channel_csc  = finfo.dim_c;
    conv_desc->kernel_channel_csc = winfo.dim_c;
    conv_desc->kernel_width_csc   = winfo.dim_w;
    conv_desc->kernel_height_csc  = winfo.dim_h;
    conv_desc->input_width_cmac   = output_Y_dims[3];
    conv_desc->input_height_cmac  = output_Y_dims[2];
    conv_desc->bytes_per_kernel   = winfo.dim_c * winfo.dim_h * winfo.dim_w * ELEMENT_SIZE;
    conv_desc->mean_ry            = 0;
    conv_desc->mean_gu            = 0;
    conv_desc->mean_bv            = 0;
    conv_desc->mean_ax            = 0;
    conv_desc->mean_format        = 0;
    conv_desc->conv_stride_x      = 1;
    conv_desc->conv_stride_y      = 1;
    conv_desc->pad_x_left         = 0;
    conv_desc->pad_x_right        = 0;
    conv_desc->pad_y_bottom       = 0;
    conv_desc->pad_y_top          = 0;
    conv_desc->dilation_x         = 1;
    conv_desc->dilation_y         = 1;
    conv_desc->pra_truncate       = 0;
    conv_desc->in_precision       = DLA_PRECISION; // INT8 vs INT16 vs FP16
    conv_desc->out_precision      = DLA_PRECISION; // INT8 vs INT16 vs FP16
    conv_desc->out_cvt.scale      = 1;
    conv_desc->out_cvt.enable     = 1;
    conv_desc->pad_val            = 0;

    // ILoadable::MemoryListEntry W_mle = m_pMeta.m_MemoryListEntries[W_mid];
    struct dla_conv_surface_desc* conv_surf = (struct dla_conv_surface_desc*)(&(conv_op->op_surf));
    conv_surf->weight_data.type             = DLA_MEM_MC;
    conv_surf->weight_data.address          = issueDlaAddr(B_mid, winfo, 1, 0, 0);
    conv_surf->weight_data.size             = B_mle.size;
    conv_surf->weight_data.width            = winfo.dim_w;
    conv_surf->weight_data.height           = winfo.dim_h;
    conv_surf->weight_data.channel          = winfo.dim_c;
    conv_surf->weight_data.line_stride      = 0;
    conv_surf->weight_data.surf_stride      = 0;
    conv_surf->weight_data.plane_stride     = 0;

    conv_surf->wmb_data.type    = DLA_MEM_HW;
    conv_surf->wmb_data.address = -1;

    conv_surf->wgs_data.type    = DLA_MEM_HW;
    conv_surf->wgs_data.address = -1;

    conv_surf->src_data.type         = DLA_MEM_MC;
    conv_surf->src_data.address      = A_mid;
    conv_surf->src_data.address      = issueDlaAddr(A_mid, finfo, 1, 0, 0);
    conv_surf->src_data.size         = A_mle.size;
    conv_surf->src_data.width        = finfo.dim_w;
    conv_surf->src_data.height       = finfo.dim_h;
    conv_surf->src_data.channel      = finfo.dim_c;
    conv_surf->src_data.line_stride  = finfo.stride_line;
    conv_surf->src_data.surf_stride  = finfo.stride_surface;
    conv_surf->src_data.plane_stride = finfo.stride_plane;

    conv_surf->dst_data.type         = DLA_MEM_MC;
    conv_surf->dst_data.address      = issueDlaAddr(Y_mid, oinfo, 1, 0, 0);
    conv_surf->dst_data.size         = Y_mle.size;
    conv_surf->dst_data.width        = oinfo.dim_w;
    conv_surf->dst_data.height       = oinfo.dim_h;
    conv_surf->dst_data.channel      = oinfo.dim_c;
    conv_surf->dst_data.line_stride  = oinfo.stride_line;
    conv_surf->dst_data.surf_stride  = oinfo.stride_surface;
    conv_surf->dst_data.plane_stride = oinfo.stride_plane;

    // Bias Add
    {
      add_op                 = new NvDlaDlaOperation();
      add_op->op_dep.op_type = DLA_OP_SDP;

      struct dla_sdp_op_desc* add_desc     = (struct dla_sdp_op_desc*)(&(add_op->op_desc));
      add_desc->src_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
      add_desc->dst_precision              = DLA_PRECISION; // INT8 vs INT16 vs FP16
      add_desc->lut_index                  = -1;
      add_desc->conv_mode                  = 0;
      add_desc->out_cvt.scale              = 1;
      add_desc->out_cvt.truncate           = 0;
      add_desc->out_cvt.enable             = 1;
      add_desc->out_cvt.offset             = 0;
      add_desc->conv_mode                  = CONV_MODE_DIRECT;
      add_desc->batch_num                  = 1;
      add_desc->batch_stride               = 0;
      add_desc->x1_op.enable               = 1;
      add_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
      add_desc->x1_op.type                 = SDP_OP_ADD;
      add_desc->x1_op.mode                 = SDP_OP_PER_KERNEL;
      add_desc->x1_op.act                  = ACTIVATION_NONE;
      add_desc->x1_op.shift_value          = 0;
      add_desc->x1_op.truncate             = 0;
      add_desc->x1_op.precision            = DLA_PRECISION; // INT8 vs INT16 vs FP16
      add_desc->x1_op.alu_operand          = 0;
      add_desc->x1_op.mul_operand          = 0; // 1; //FIXME
      add_desc->x1_op.cvt.alu_cvt.scale    = 0;
      add_desc->x1_op.cvt.alu_cvt.truncate = 0;
      add_desc->x1_op.cvt.alu_cvt.enable   = 0;
      add_desc->x1_op.cvt.alu_cvt.offset   = 0;
      add_desc->x1_op.cvt.mul_cvt.scale    = 0;
      add_desc->x1_op.cvt.mul_cvt.truncate = 0;
      add_desc->x1_op.cvt.mul_cvt.enable   = 0;
      add_desc->x1_op.cvt.mul_cvt.offset   = 0;
      add_desc->x2_op.alu_type             = SDP_ALU_OP_SUM; // FIXME: since x2_op.enable is 0, I don't think it needs any paticular
                                                 // value. This value is from nv_small_alexnet.nvdla.
      add_desc->y_op.alu_type = SDP_ALU_OP_SUM;

      struct dla_sdp_surface_desc* add_surf = (struct dla_sdp_surface_desc*)(&(add_op->op_surf));
      add_surf->src_data.type               = DLA_MEM_HW;
      add_surf->src_data.address            = -1;
#if DLA_NV_SMALL
      add_surf->src_data.size = 0; // FIXME: in nv_small_alexnet.nvdla it's 0. In KMD, I don't see where to use this
                                   // value. Seems this value does not impact anything in KMD.
#else
      add_surf->src_data.size = conv_surf->dst_data.size;
#endif
      add_surf->src_data.width   = conv_surf->dst_data.width;
      add_surf->src_data.height  = conv_surf->dst_data.height;
      add_surf->src_data.channel = conv_surf->dst_data.channel;
#if DLA_NV_SMALL
      add_surf->src_data.line_stride  = 0; // oinfo.stride_line;
      add_surf->src_data.surf_stride  = 0; // oinfo.stride_surface;
      add_surf->src_data.plane_stride = 0; // oinfo.stride_plane;
#else
      add_surf->src_data.line_stride = oinfo.stride_line;
      add_surf->src_data.surf_stride = oinfo.stride_surface;
      add_surf->src_data.plane_stride = oinfo.stride_plane;
#endif
      add_surf->x1_data.type         = DLA_MEM_MC;
      add_surf->x1_data.address      = issueDlaAddr(C_mid, binfo, 1, 0, 0);
      add_surf->x1_data.size         = C_mle.size;
      add_surf->x1_data.width        = input_C_dims[3];
      add_surf->x1_data.height       = input_C_dims[2];
      add_surf->x1_data.channel      = input_C_dims[0];
      add_surf->x1_data.line_stride  = binfo.stride_line;
      add_surf->x1_data.surf_stride  = binfo.stride_surface;
      add_surf->x1_data.plane_stride = binfo.stride_plane;

      add_surf->dst_data.type         = DLA_MEM_MC;
      add_surf->dst_data.address      = conv_surf->dst_data.address;
      add_surf->dst_data.size         = conv_surf->dst_data.size;
      add_surf->dst_data.width        = conv_surf->dst_data.width;
      add_surf->dst_data.height       = conv_surf->dst_data.height;
      add_surf->dst_data.channel      = conv_surf->dst_data.channel;
      add_surf->dst_data.line_stride  = conv_surf->dst_data.line_stride;
      add_surf->dst_data.surf_stride  = conv_surf->dst_data.surf_stride;
      add_surf->dst_data.plane_stride = conv_surf->dst_data.plane_stride;

      conv_surf->dst_data.type    = DLA_MEM_HW;
      conv_surf->dst_data.address = -1;
#if DLA_NV_SMALL
      conv_surf->dst_data.size = 0; // FIXME:
#endif
      // conv_surf->dst_data.line_stride = 0;
      // conv_surf->dst_data.surf_stride = 0;
      // conv_surf->dst_data.plane_stride = 0;
    }
    issueDlaOp(conv_op, add_op, m_pMeta.m_pPrevOp);
  }
}

void CodeEmitVisitor::visit(const Softmax& pOp)
{
  pOp.print(errs());
  errs() << "\n";
  m_pMeta.m_DlaAddresses = m_pMeta.m_AddressListEntries.size();

  const Tensor* input_input_t = pOp.getInput(0);
  // void *input_input = m_ATable[input_input_t];
  int32_t input_input_ndim    = input_input_t->getNumOfDimensions();
  int32_t input_input_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_input_ndim; ++i)
    input_input_dims[i] = input_input_t->dimension(i);
  int                        input_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_input_t];
  ILoadable::MemoryListEntry input_mle = m_pMeta.m_MemoryListEntries[input_mid];
  NvDlaCubeInfo              iinfo(NVDLA_CUBE_FEATURE, input_input_dims[0], input_input_dims[1], input_input_dims[2],
                      input_input_dims[3]);

  const Tensor* output_output_t = pOp.getOutput(0);
  // void *output_output = m_ATable[output_output_t];
  int32_t output_output_ndim    = output_output_t->getNumOfDimensions();
  int32_t output_output_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_output_ndim; ++i)
    output_output_dims[i] = output_output_t->dimension(i);
  int                        output_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_output_t];
  ILoadable::MemoryListEntry output_mle = m_pMeta.m_MemoryListEntries[output_mid];
  NvDlaCubeInfo oinfo(NVDLA_CUBE_FEATURE, output_output_dims[0], output_output_dims[1], output_output_dims[2],
                      output_output_dims[3]);

  int32_t axis = pOp.getAxis().value();

  NvDlaEmuOperation*          softmax_op = new NvDlaEmuOperation();
  struct emu_softmax_op_desc* op_desc    = (struct emu_softmax_op_desc*)(&(softmax_op->op_desc));
  op_desc->common.op_type                = NVDLA_EMU_OP_SOFTMAX;
  op_desc->axis                          = axis;

  struct emu_softmax_buffer_descs* op_buf = (struct emu_softmax_buffer_descs*)(&(softmax_op->op_buf));
  op_buf->src_data.addressIndex           = issueEmuAddr(input_mid);
  op_buf->src_data.size                   = input_mle.size;
  op_buf->src_data.format                 = DLA_PRECISION; // INT8 vs INT16 vs FP16
  op_buf->src_data.width                  = input_input_dims[3];
  op_buf->src_data.height                 = input_input_dims[2];
  op_buf->src_data.channel                = input_input_dims[1];
  op_buf->src_data.line_stride            = iinfo.stride_line;
  op_buf->src_data.surf_stride            = iinfo.stride_surface;
  NVDLA_DBG("softmax in(sz:%d w:%d h:%d c:%d ls:%d ss%d)\n", input_mle.size, input_input_dims[3], input_input_dims[2],
            input_input_dims[1], iinfo.stride_line, iinfo.stride_surface);

  op_buf->dst_data.addressIndex = issueEmuAddr(output_mid);
  op_buf->dst_data.size         = output_mle.size;
  op_buf->dst_data.format       = DLA_PRECISION; // INT8 vs INT16 vs FP16
  op_buf->dst_data.width        = output_output_dims[3];
  op_buf->dst_data.height       = output_output_dims[2];
  op_buf->dst_data.channel      = output_output_dims[1];
  op_buf->dst_data.line_stride  = oinfo.stride_line;
  op_buf->dst_data.surf_stride  = oinfo.stride_surface;
  NVDLA_DBG("softmax out(sz:%d w:%d h:%d c:%d ls:%d ss%d)\n", output_mle.size, output_output_dims[3],
            output_output_dims[2], output_output_dims[1], oinfo.stride_line, oinfo.stride_surface);
  issueEmuOp(softmax_op);
}

void CodeEmitVisitor::visit(const Concat& pOp)
{
  // Prepare input
  int32_t  input_inputs_ntensor = pOp.getNumOfInputs() - 0;
  void*    input_inputs[input_inputs_ntensor];
  int32_t  input_inputs_ndim[input_inputs_ntensor];
  int32_t* input_inputs_dims[input_inputs_ntensor];
  for (int i = 0; i < input_inputs_ntensor; ++i) {
    // input_inputs[i] = m_ATable[pOp.getInput(0 + i)];
    input_inputs_ndim[i] = pOp.getInput(0 + i)->getNumOfDimensions();
    input_inputs_dims[i] = new int32_t[4];
    for (int32_t j = 0; j < 4; ++j) {
      input_inputs_dims[i][j] = (j < input_inputs_ndim[i]) ? pOp.getInput(0 + i)->dimension(j) : 1;
    }
    NVDLA_DBG("Concat input[%d](%d %d %d %d)\n", i, input_inputs_dims[i][0], input_inputs_dims[i][1],
              input_inputs_dims[i][2], input_inputs_dims[i][3]);
  }

  // Prepare output
  const Tensor* output_concat_result_t = pOp.getOutput(0);
  // void *output_concat_result = m_ATable[output_concat_result_t];
  int32_t output_concat_result_ndim    = output_concat_result_t->getNumOfDimensions();
  int32_t output_concat_result_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_concat_result_ndim; ++i)
    output_concat_result_dims[i] = output_concat_result_t->dimension(i);
  NVDLA_DBG("Concat output(%d %d %d %d)\n", output_concat_result_dims[0], output_concat_result_dims[1],
            output_concat_result_dims[2], output_concat_result_dims[3]);
  int                        output_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_concat_result_t];
  ILoadable::MemoryListEntry output_mle = m_pMeta.m_MemoryListEntries[output_mid];

  // Prepare attributesinput_inputs_ndim
  int32_t axis = pOp.getAxis().value();
  NVDLA_DBG("Concat AXIS[%d]\n", axis);

  if (axis == 1) {
  } else {
    // critical error
  }

  // Clean
  for (int i = 0; i < input_inputs_ntensor; ++i) {
    delete[] input_inputs_dims[i];
  }
}

void CodeEmitVisitor::visit(const NvDlaConvRelu& pConvRelu)
{
  printf("------------------> Hello! ConvRelu is here.\n");
  pConvRelu.m_Conv.print(errs());
  errs() << "\n";
  pConvRelu.m_Relu.print(errs());
  errs() << "\n";
  printf("=================== Hello! ConvRelu is here.\n");
  const Conv& pOp = pConvRelu.m_Conv;
  pOp.print(errs());
  errs() << "\n";

  // Current assumptions:
  // -- CONV has Bias; CONV uses SDP to compute Bias.
  // -- That SDP can conveniently take care of ReLU as well.
  // FIXME: If CONV has no Bias hence does not instantiate SDP, ReLU won't be computed!

  // There can be multiple groups
  // Within a group, one conv model layer can be split into multiple conv hardware layer
  // Each hardware layer can add two elements into m_pMeta.m_DLAOperationList

  int size_before_conv = m_pMeta.m_DLAOperationList.size();
  visit(pConvRelu.m_Conv);
  // visit(pConvRelu.m_Relu); // All related SDP settings need to be done by Conv
  int size_after_conv = m_pMeta.m_DLAOperationList.size();
  for (int pos = size_before_conv; pos < size_after_conv; pos++) {
    NvDlaDlaOperation *add_op = m_pMeta.m_DLAOperationList[pos];
    if (add_op->op_dep.op_type == DLA_OP_SDP) {
       struct dla_sdp_op_desc *add_desc = (struct dla_sdp_op_desc *)(&(add_op->op_desc));
       add_desc->x1_op.act = ACTIVATION_RELU;
       add_desc->x1_op.mode = SDP_OP_PER_KERNEL;
    }
  }
}

void CodeEmitVisitor::visit(const NvDlaMaxPool& pMaxPool)
{
  printf("------------------> Hello! MaxPool is here.\n");
  pMaxPool.print(errs());
  errs() << "\n";
  printf("=================== Hello! MaxPool is here.\n");

  // FIXME: copied from visit(MaxPool), which is a redundancy.
  const MaxPool& pOp = pMaxPool.m_MaxPool;
  pOp.print(errs());
  errs() << "\n";

  const Tensor* input_X_t = pOp.getInput(0);
  // void *input_X = m_ATable[input_X_t];
  int32_t input_X_ndim    = input_X_t->getNumOfDimensions();
  int32_t input_X_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_X_ndim; ++i)
    input_X_dims[i] = input_X_t->dimension(i);
  int                        X_mid = m_pMeta.m_MemIdxTable[(Tensor*)input_X_t];
  ILoadable::MemoryListEntry X_mle = m_pMeta.m_MemoryListEntries[X_mid];
  NvDlaCubeInfo X_cube(NVDLA_CUBE_FEATURE, input_X_dims[0], input_X_dims[1], input_X_dims[2], input_X_dims[3]);

  // Prepare output
  const Tensor* output_Y_t = pOp.getOutput(0);
  // void *output_Y = m_ATable[output_Y_t];
  int32_t output_Y_ndim    = output_Y_t->getNumOfDimensions();
  int32_t output_Y_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Y_ndim; ++i)
    output_Y_dims[i] = output_Y_t->dimension(i);
  int                        Y_mid = m_pMeta.m_MemIdxTable[(Tensor*)output_Y_t];
  ILoadable::MemoryListEntry Y_mle = m_pMeta.m_MemoryListEntries[Y_mid];
  NvDlaCubeInfo Y_cube(NVDLA_CUBE_FEATURE, output_Y_dims[0], output_Y_dims[1], output_Y_dims[2], output_Y_dims[3]);

  const Tensor* output_Indices_t    = NULL;
  void*         output_Indices      = NULL;
  int32_t       output_Indices_ndim = 0;
  if (pOp.getNumOfOutputs() > 1) {
    output_Indices_t = pOp.getOutput(1);
    // output_Indices = m_ATable[output_Indices_t];
    output_Indices_ndim = output_Indices_t->getNumOfDimensions();
  }
  int32_t output_Indices_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_Indices_ndim; ++i)
    output_Indices_dims[i] = output_Indices_t->dimension(i);

  // Prepare attributes
  // const char * auto_pad = pOp.getAutoPad().value().c_str();
  int32_t number_of_kernel_shape = pOp.getKernelShape().vector().size();
  int32_t kernel_shape[number_of_kernel_shape];
  for (int i = 0; i < number_of_kernel_shape; ++i)
    kernel_shape[i] = pOp.getKernelShape().at(i);
  int32_t number_of_pads = pOp.getPads().vector().size();
  int32_t pad_shapes[number_of_pads];
  for (int i = 0; i < number_of_pads; ++i)
    pad_shapes[i] = pOp.getPads().at(i);
  int32_t storage_order     = pOp.getStorageOrder().value();
  int32_t number_of_strides = pOp.getStrides().vector().size();
  int32_t strides[number_of_strides];
  for (int i = 0; i < number_of_strides; ++i)
    strides[i] = pOp.getStrides().at(i);

  int32_t group = pMaxPool.m_Group.value();
#if 0
  if (group > 1) {
    input_X_dims[1] /= group;
    output_Y_dims[1] /= group;
  }
#endif

  for (int g = 0; g < group; ++g) {
    NvDlaDlaOperation* maxpool_op = new NvDlaDlaOperation();
    maxpool_op->op_dep.op_type    = DLA_OP_PDP;

    struct dla_pdp_op_desc* maxpool_desc = (struct dla_pdp_op_desc*)(&(maxpool_op->op_desc));
    maxpool_desc->partial_in_width_first = X_cube.dim_w; // FIXME: what does this mean in hardware?
    maxpool_desc->partial_in_width_mid  = 0;
    maxpool_desc->partial_in_width_last = 0;
    maxpool_desc->partial_width_first = Y_cube.dim_w; // FIXME: what does this mean in hardware?
    maxpool_desc->partial_width_mid  = 0;
    maxpool_desc->partial_width_last = 0;
    maxpool_desc->split_num          = 1;
    maxpool_desc->pool_mode          = POOL_MODE_MAX;
    maxpool_desc->pool_width         = kernel_shape[1] - 1;
    maxpool_desc->pool_height        = kernel_shape[0] - 1;
    maxpool_desc->stride_x           = strides[1];
    maxpool_desc->stride_y           = strides[0];
    maxpool_desc->pad_top            = pad_shapes[0]; // pad_shape - H
    maxpool_desc->pad_left           = pad_shapes[1]; // pad_shape - W
    maxpool_desc->pad_bottom         = pad_shapes[2];
    maxpool_desc->pad_right          = pad_shapes[3];

    maxpool_desc->precision = DLA_PRECISION;

    struct dla_pdp_surface_desc* maxpool_surf = (struct dla_pdp_surface_desc*)(&(maxpool_op->op_surf));
    maxpool_surf->src_data.type               = DLA_MEM_MC;
    maxpool_surf->src_data.address            = issueDlaAddr(X_mid, X_cube, group, g, 0);
    maxpool_surf->src_data.size = X_mle.size / group; // FIXME: what if x_mle.size is odd?
    maxpool_surf->src_data.width  = X_cube.dim_w;
    maxpool_surf->src_data.height = X_cube.dim_h;
    maxpool_surf->src_data.channel = X_cube.dim_c / group; // FIXME: what if X_cube.dim_c is odd?
    maxpool_surf->src_data.line_stride  = X_cube.stride_line;
    maxpool_surf->src_data.surf_stride  = X_cube.stride_surface;
    maxpool_surf->src_data.plane_stride = X_cube.stride_plane;

    maxpool_surf->dst_data.type    = DLA_MEM_MC;
    maxpool_surf->dst_data.address = issueDlaAddr(Y_mid, Y_cube, group, g, 0);
    maxpool_surf->dst_data.size = Y_mle.size / group; // FIXME: what if Y_mle.size is odd?
    maxpool_surf->dst_data.width  = Y_cube.dim_w;
    maxpool_surf->dst_data.height = Y_cube.dim_h;
    maxpool_surf->dst_data.channel = Y_cube.dim_c / group; // FIXME: what if Y_cube.dim_c is odd?
    maxpool_surf->dst_data.line_stride  = Y_cube.stride_line;
    maxpool_surf->dst_data.surf_stride  = Y_cube.stride_surface;
    maxpool_surf->dst_data.plane_stride = Y_cube.stride_plane;

    issueDlaOp(maxpool_op, NULL, m_pMeta.m_pPrevOp);
  }
}

void CodeEmitVisitor::visit(const NvDlaConvReluMaxPool& pConvReluMaxPool)
{
  printf("------------------> Hello! ConvReluMaxPool is here.\n");
  pConvReluMaxPool.print(errs());
  errs() << "\n";
  pConvReluMaxPool.m_Conv.print(errs());
  errs() << "\n";
  pConvReluMaxPool.m_Relu.print(errs());
  errs() << "\n";
  pConvReluMaxPool.m_MaxPool.print(errs());
  errs() << "\n";
  printf("=================== Hello! ConvReluMaxPool is here.\n");
#if 0
  const Conv& pOp = pConvRelu.m_Conv;
  pOp.print(errs());
  errs() << "\n";

  // Current assumptions:
  // -- CONV has Bias; CONV uses SDP to compute Bias.
  // -- That SDP can conveniently take care of ReLU as well.
  // FIXME: If CONV has no Bias hence does not instantiate SDP, ReLU won't be computed!

  // There can be multiple groups
  // Within a group, one conv model layer can be split into multiple conv hardware layer
  // Each hardware layer can add two elements into m_pMeta.m_DLAOperationList

  int size_before_conv = m_pMeta.m_DLAOperationList.size();
  visit(pConvRelu.m_Conv);
  // visit(pConvRelu.m_Relu); // All related SDP settings need to be done by Conv
  int size_after_conv = m_pMeta.m_DLAOperationList.size();
  for (int pos = size_before_conv; pos < size_after_conv; pos++) {
    NvDlaDlaOperation *add_op = m_pMeta.m_DLAOperationList[pos];
    if (add_op->op_dep.op_type == DLA_OP_SDP) {
       struct dla_sdp_op_desc *add_desc = (struct dla_sdp_op_desc *)(&(add_op->op_desc));
       add_desc->x1_op.act = ACTIVATION_RELU;
       add_desc->x1_op.mode = SDP_OP_PER_KERNEL;
    }
  }
#endif
}

void CodeEmitVisitor::visit(const NvDlaGemmRelu& pGemmRelu)
{
  printf("------------------> Hello! GemmRelu is here.\n");
  pGemmRelu.m_Gemm.print(errs());
  errs() << "\n";
  pGemmRelu.m_Relu.print(errs());
  errs() << "\n";
  printf("=================== Hello! GemmRelu is here.\n");
  const Gemm& pOp = pGemmRelu.m_Gemm;
  pOp.print(errs());
  errs() << "\n";

  // Current assumptions:
  // -- CONV has Bias; CONV uses SDP to compute Bias.
  // -- That SDP can conveniently take care of ReLU as well.
  // FIXME: If CONV has no Bias hence does not instantiate SDP, ReLU won't be computed!

  // There can be multiple groups
  // Within a group, one conv model layer can be split into multiple conv hardware layer
  // Each hardware layer can add two elements into m_pMeta.m_DLAOperationList

  int size_before_conv = m_pMeta.m_DLAOperationList.size();
  visit(pGemmRelu.m_Gemm);
  // visit(pGemmRelu.m_Relu); // All related SDP settings need to be done by Gemm
  int size_after_conv = m_pMeta.m_DLAOperationList.size();
  for (int pos = size_before_conv; pos < size_after_conv; pos++) {
    NvDlaDlaOperation *add_op = m_pMeta.m_DLAOperationList[pos];
    if (add_op->op_dep.op_type == DLA_OP_SDP) {
       struct dla_sdp_op_desc *add_desc = (struct dla_sdp_op_desc *)(&(add_op->op_desc));
       add_desc->x1_op.act = ACTIVATION_RELU;
       add_desc->x1_op.mode = SDP_OP_PER_KERNEL;
    }
  }
}

int CodeEmitVisitor::packWeight(const Tensor* t, int dims[4], int gidx, bool shouldPadZero)
{
  std::string blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);

  ILoadable::Blob b;
  b.name = blob_name;
  b.size =
    (dims[0] * dims[1] * dims[2] * dims[3] * ELEMENT_SIZE + WEIGHT_ATOM_CUBE_SIZE - 1) & ~(WEIGHT_ATOM_CUBE_SIZE - 1);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;
  // b.data = blob_data;
  NvU8* blob_data = new NvU8[b.size];
  memset(blob_data, 0, b.size);
  if (0) {
    FILE* fptr = fopen((blob_name + ".bin").c_str(), "wb");
    fwrite(static_cast<const FloatTensor*>(t)->getValues().data(),
           dims[0] * dims[1] * dims[2] * dims[3] * sizeof(float), 1, fptr);
    fclose(fptr);
  }
  // weight_pack(blob_data, m_pMeta.m_WeightTable[input_W_t], group, g, input_W_dims[0], input_W_dims[1],
  // input_W_dims[2], input_W_dims[3], 0);
  weight_pack(blob_data, (float*)(static_cast<const FloatTensor*>(t)->getValues().data()), gidx, dims, 0, shouldPadZero);

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  ILoadable::MemoryListEntry mle;
  mle.id             = m_pMeta.m_MemoryListEntries.size();
  mle.size           = b.size;
  mle.alignment      = 4096;
  mle.flags          = ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET;
  mle.domain         = ILoadable::MemoryDomain_SYSMEM;
  mle.bind_id        = 0;
  mle.tensor_desc_id = 0;
  mle.contents.push_back(blob_name);
  mle.offsets.push_back(0);

  m_pMeta.m_MemoryListEntries.push_back(mle);
  return mle.id;
}

int CodeEmitVisitor::packBias(const Tensor* t, int dims[4], int gidx)
{
  if (dims[1] != 1 && dims[2] != 1 && dims[3] != 1) {
    // critical error
    NVDLA_DBG("PACK FEATURE, FEATURE IS NOT 1D ARRAY ##################################");
  }
  std::string   blob_name = "tb-" + std::to_string(m_pMeta.m_NumBlobs++);
  NvDlaCubeInfo finfo(NVDLA_CUBE_FEATURE, 1, dims[0], dims[2], dims[3]);

  ILoadable::Blob b;
  b.name              = blob_name;
  b.size              = (dims[0] * ELEMENT_SIZE + FEATURE_ATOM_CUBE_SIZE - 1) & ~(FEATURE_ATOM_CUBE_SIZE - 1);
  b.version.major     = 0;
  b.version.minor     = 0;
  b.version.sub_minor = 0;
  b.interface         = ILoadable::Interface_NONE;
  b.subInterface      = 0;
  // b.data = blob_data;
  NvU8* blob_data = new NvU8[b.size];
  memset(blob_data, 0, b.size);
  nv_weight_t* dest    = (nv_weight_t*)blob_data;
  float*       data    = (float*)(static_cast<const FloatTensor*>(t)->getValues().data());
  int          group_c = gidx * dims[0];
  for (int c = 0; c < dims[0]; c++) {
    *(dest + c) = (nv_weight_t)__gnu_f2h_ieee(*(data + group_c + c)); // FIXME: bad hack method.
  }
#if 0
  for(int h = 0; h < dims[2]; h++){
    for(int w = 0; w < dims[3]; w++){
      for(int c = 0; c < dims[1]; c++){
        int surf_idx = (c+group_c) / (32/sizeof(short));
        int c_ofs = (c+group_c) % (32/sizeof(short));
        int dest_ofs =
          surf_idx*(finfo.stride_surface/sizeof(short)) +
          h*(finfo.stride_line/sizeof(short)) +
          w*(32/sizeof(short)) +
          c_ofs;
        int data_ofs = c*dims[2]*dims[3] + h*dims[3] + w;
        *(dest + dest_ofs) = __gnu_f2h_ieee(*(data+data_ofs));//FIXME: bad hack method
      }
    }
  }
#endif

  m_pMeta.m_Loadable.priv()->setSymbolContent(blob_name, b, blob_data);

  ILoadable::MemoryListEntry mle;
  mle.id             = m_pMeta.m_MemoryListEntries.size();
  mle.size           = b.size;
  mle.alignment      = 4096; // FIXME: what does 4097 mean?
  mle.flags          = ILoadable::MemoryFlags_ALLOC | ILoadable::MemoryFlags_SET;
  mle.domain         = ILoadable::MemoryDomain_SYSMEM;
  mle.bind_id        = 0;
  mle.tensor_desc_id = 0;
  mle.contents.push_back(blob_name);
  mle.offsets.push_back(0);

  m_pMeta.m_MemoryListEntries.push_back(mle);
  return mle.id;
}

int CodeEmitVisitor::issueEmuAddr(int mid)
{
  int aid = m_pMeta.m_AddressListEntries.size();

  ILoadable::AddressListEntry ale;
  ILoadable::MemoryListEntry  mle = m_pMeta.m_MemoryListEntries[mid];

  ale.size   = 0;
  ale.offset = 0;
  ale.mem_id = mid;
  ale.id     = aid;

  NVDLA_DBG("AddressEntry s:%9d o:%9d mid:%3d id:%3d\n", ale.size, ale.offset, ale.mem_id, ale.id);
  m_pMeta.m_AddressListEntries.push_back(ale);
  return aid;
}

void CodeEmitVisitor::issueEmuOp(NvDlaEmuOperation* op) { m_pMeta.m_EMUOperationList.push_back(op); }

int CodeEmitVisitor::issueDlaAddr(int mid, NvDlaCubeInfo cube, int groups, int gidx, int ofs)
{
  int aid = m_pMeta.m_AddressListEntries.size();

  ILoadable::AddressListEntry ale;
  ILoadable::MemoryListEntry  mle = m_pMeta.m_MemoryListEntries[mid];

  ale.size = mle.size;
  if (groups >= 0) {
    int h_offset = ofs * cube.stride_line;
    ale.offset   = ((gidx * (cube.dim_n * cube.dim_c * cube.dim_h * cube.dim_w * ELEMENT_SIZE)) / groups) + h_offset;
    // ale.offset = h_offset;
  } else { // FIXME: when does groups < 0?
    int surf_offset = ofs / (FEATURE_ATOM_CUBE_SIZE / ELEMENT_SIZE);
    ale.offset      = surf_offset * cube.stride_surface;
  }
  ale.mem_id = mid;
  ale.id     = aid;
  NVDLA_DBG("cube(%d %d %d %d %d), group(%d/%d) ofs %d\n", cube.dim_n, cube.dim_c, cube.dim_h, cube.dim_w, ELEMENT_SIZE,
            groups, gidx, ofs);
  NVDLA_DBG("AddressEntry s:%9d o:%9d mid:%3d id:%3d\n", ale.size, ale.offset, ale.mem_id, ale.id);

  m_pMeta.m_AddressListEntries.push_back(ale);
  return aid;
}

void CodeEmitVisitor::issueDlaOp(NvDlaDlaOperation* op, NvDlaDlaOperation* op_fuse, NvDlaDlaOperation* op_prev)
{
  // NvDlaDlaOperation *conv_op = new NvDlaDlaOperation();
  struct dla_common_op_desc* op_desc = &(op->op_dep);
  int                        op_type = op_desc->op_type;
  NVDLA_DBG("issueDlaOp: %d\n", op_type);
  op_desc->index            = m_pMeta.m_DLAOperationList.size();
  op_desc->roi_index        = 0;
  op_desc->dependency_count = 0;

  if (op_prev != NULL) {
    struct dla_common_op_desc* prev_op_desc = &(op_prev->op_dep);
    prev_op_desc->consumers[op_type].index  = op_desc->index;
    prev_op_desc->consumers[op_type].event  = 1;
    op_desc->dependency_count++;
  }

  if (m_pMeta.m_pDepOp[op_type] != NULL) {
    struct dla_common_op_desc* dep_op_desc = &(m_pMeta.m_pDepOp[op_type]->op_dep);
    // NvDlaDlaOperation *dep_op = m_pMeta.m_pDepOp[op_type];
    if (m_pMeta.m_pDepOp[op_type] != op_prev) {
      dep_op_desc->consumers[op_type].index = op_desc->index;
      dep_op_desc->consumers[op_type].event = 2;
      op_desc->dependency_count++;
    }
  }

  m_pMeta.m_DlaNetworkDesc.op_head[op_type] = (m_pMeta.m_DlaNetworkDesc.op_head[op_type] < 0)
                                                ? m_pMeta.m_DLAOperationList.size()
                                                : m_pMeta.m_DlaNetworkDesc.op_head[op_type];
  m_pMeta.m_DLAOperationList.push_back(op);
  m_pMeta.m_pDepOp[op_type] = op;
  // NVDLA_DBG("op conv - %p\n", conv_op);

  if (op_fuse != NULL) {
    struct dla_common_op_desc* fuse_op_desc = &(op_fuse->op_dep);
    // TODO: Fix dst address in conv_op
    int op_fuse_type               = fuse_op_desc->op_type;
    fuse_op_desc->index            = m_pMeta.m_DLAOperationList.size();
    fuse_op_desc->roi_index        = 0;
    fuse_op_desc->dependency_count = 1;

    fuse_op_desc->fused_parent.index = op_desc->index;
    fuse_op_desc->fused_parent.event = 3;

    op_desc->consumers[op_fuse_type].index = fuse_op_desc->index;
    op_desc->consumers[op_fuse_type].event = 2;
    if (op_prev != NULL) {
      struct dla_common_op_desc* prev_op_desc = &(op_prev->op_dep);
      prev_op_desc->consumers[op_type].event  = 1;
    }
    op_desc->dependency_count++;

    if (m_pMeta.m_pDepOp[op_fuse_type] != NULL) {
      struct dla_common_op_desc* dep_op_desc     = &(m_pMeta.m_pDepOp[op_fuse_type]->op_dep);
      dep_op_desc->consumers[op_fuse_type].index = fuse_op_desc->index;
      dep_op_desc->consumers[op_fuse_type].event = 2;

      fuse_op_desc->dependency_count++;
    }
    m_pMeta.m_pDepOp[op_fuse_type] = op_fuse;

    // NVDLA_DBG("op add - %p\n", add_op);
    m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type] = (m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type] < 0)
                                                       ? m_pMeta.m_DLAOperationList.size()
                                                       : m_pMeta.m_DlaNetworkDesc.op_head[op_fuse_type];
    m_pMeta.m_DLAOperationList.push_back(op_fuse);
    printf ("DBGF: op_type %d fused with op_fuse_type %d\n", op_type, op_fuse_type);
    m_pMeta.m_pPrevOp = op_fuse;
  } else {
    m_pMeta.m_pPrevOp = op;
  }
}

void CodeEmitVisitor::visit(const Sum& pSum)
{
  pSum.print(errs());
  errs() << "\n";

  const Tensor* output_value   = pSum.getOutput(0);
  int32_t       output_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < output_value->getNumOfDimensions(); ++i) {
    output_dims[i] = output_value->dimension(i);
  }

  int                        output_mid = m_pMeta.m_MemIdxTable[const_cast<Tensor*>(output_value)];
  ILoadable::MemoryListEntry output_mle = m_pMeta.m_MemoryListEntries[output_mid];
  NvDlaCubeInfo output_cube(NVDLA_CUBE_FEATURE, output_dims[0], output_dims[1], output_dims[2], output_dims[3]);

  NvDlaDlaOperation* relu_op = new NvDlaDlaOperation();
  relu_op->op_dep.op_type    = DLA_OP_SDP;

  struct dla_sdp_op_desc* op_desc     = (struct dla_sdp_op_desc*)(&(relu_op->op_desc));
  op_desc->src_precision              = PRECISION_FP16;
  op_desc->dst_precision              = PRECISION_FP16;
  op_desc->lut_index                  = -1;
  op_desc->conv_mode                  = 0;
  op_desc->out_cvt.scale              = 1;
  op_desc->out_cvt.truncate           = 0;
  op_desc->out_cvt.enable             = 1;
  op_desc->out_cvt.offset             = 0;
  op_desc->conv_mode                  = CONV_MODE_DIRECT;
  op_desc->batch_num                  = 1;
  op_desc->batch_stride               = 0;
  op_desc->x1_op.enable               = 1;
  op_desc->x1_op.alu_type             = SDP_ALU_OP_SUM;
  op_desc->x1_op.type                 = SDP_OP_ADD;
  op_desc->x1_op.mode                 = SDP_OP_PER_POINT;
  op_desc->x1_op.act                  = ACTIVATION_NONE;
  op_desc->x1_op.shift_value          = 0;
  op_desc->x1_op.truncate             = 0;
  op_desc->x1_op.precision            = PRECISION_FP16;
  op_desc->x1_op.alu_operand          = 0;
  op_desc->x1_op.mul_operand          = 1;
  op_desc->x1_op.cvt.alu_cvt.scale    = 0;
  op_desc->x1_op.cvt.alu_cvt.truncate = 0;
  op_desc->x1_op.cvt.alu_cvt.enable   = 0;
  op_desc->x1_op.cvt.alu_cvt.offset   = 0;
  op_desc->x1_op.cvt.mul_cvt.scale    = 0;
  op_desc->x1_op.cvt.mul_cvt.truncate = 0;
  op_desc->x1_op.cvt.mul_cvt.enable   = 0;
  op_desc->x1_op.cvt.mul_cvt.offset   = 0;

  struct dla_sdp_surface_desc* surf_desc = (struct dla_sdp_surface_desc*)(&(relu_op->op_surf));

  const Tensor* input_left_t       = pSum.getInput(0);
  int32_t       input_left_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_left_t->getNumOfDimensions(); ++i) {
    input_left_dims[i] = input_left_t->dimension(i);
  }
  int                        left_mid = m_pMeta.m_MemIdxTable[const_cast<Tensor*>(input_left_t)];
  ILoadable::MemoryListEntry left_mle = m_pMeta.m_MemoryListEntries[left_mid];
  NvDlaCubeInfo              left_cube(NVDLA_CUBE_FEATURE, input_left_dims[0], input_left_dims[1], input_left_dims[2],
                          input_left_dims[3]);

  surf_desc->src_data.type         = DLA_MEM_MC;
  surf_desc->src_data.address      = issueDlaAddr(left_mid, left_cube, 1, 0, 0);
  surf_desc->src_data.size         = left_mle.size;
  surf_desc->src_data.width        = left_cube.dim_w;
  surf_desc->src_data.height       = left_cube.dim_h;
  surf_desc->src_data.channel      = left_cube.dim_c;
  surf_desc->src_data.line_stride  = left_cube.stride_line;
  surf_desc->src_data.surf_stride  = left_cube.stride_surface;
  surf_desc->src_data.plane_stride = left_cube.stride_plane;

  const Tensor* input_right_t       = pSum.getInput(1);
  int32_t       input_right_dims[4] = {1, 1, 1, 1};
  for (int i = 0; i < input_right_t->getNumOfDimensions(); ++i) {
    input_right_dims[i] = input_right_t->dimension(i);
  }
  int                        right_mid = m_pMeta.m_MemIdxTable[const_cast<Tensor*>(input_right_t)];
  ILoadable::MemoryListEntry right_mle = m_pMeta.m_MemoryListEntries[right_mid];
  NvDlaCubeInfo right_cube(NVDLA_CUBE_FEATURE, input_right_dims[0], input_right_dims[1], input_right_dims[2],
                           input_right_dims[3]);

  surf_desc->x1_data.type         = DLA_MEM_MC;
  surf_desc->x1_data.address      = issueDlaAddr(right_mid, right_cube, 1, 0, 0);
  surf_desc->x1_data.size         = right_mle.size;
  surf_desc->x1_data.width        = right_cube.dim_w;
  surf_desc->x1_data.height       = right_cube.dim_h;
  surf_desc->x1_data.channel      = right_cube.dim_c;
  surf_desc->x1_data.line_stride  = right_cube.stride_line;
  surf_desc->x1_data.surf_stride  = right_cube.stride_surface;
  surf_desc->x1_data.plane_stride = right_cube.stride_plane;

  surf_desc->dst_data.type         = DLA_MEM_MC;
  surf_desc->dst_data.address      = issueDlaAddr(output_mid, output_cube, 1, 0, 0);
  surf_desc->dst_data.size         = output_mle.size;
  surf_desc->dst_data.width        = output_cube.dim_w;
  surf_desc->dst_data.height       = output_cube.dim_h;
  surf_desc->dst_data.channel      = output_cube.dim_c;
  surf_desc->dst_data.line_stride  = output_cube.stride_line;
  surf_desc->dst_data.surf_stride  = output_cube.stride_surface;
  surf_desc->dst_data.plane_stride = output_cube.stride_plane;

  issueDlaOp(relu_op, NULL, m_pMeta.m_pPrevOp);
}
