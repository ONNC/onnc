#ifndef BM188X_BMKERNEL_API_H
#define BM188X_BMKERNEL_API_H

#include <onnc/Target/TG/BM188x/asm/bm188x_asm.pb.h>
#include <onnc/Target/TG/bmkernel_api_base.h>

namespace bmnet {

namespace bmnet_asm {

// clang-format off
inline void bmnet_pooling_fixed_forward_bmkernel(
    gaddr_t ifmap_gaddr,
    gaddr_t ofmap_gaddr,
    gaddr_t index_gaddr,
    gaddr_t o_findex_gaddr,
    int n,
    int c,
    int h,
    int w,
    int kh,
    int kw,
    int pad_top,
    int pad_bot,
    int pad_left,
    int pad_right,
    int stride_h,
    int stride_w,
    int is_avg_pooling,
    float avg_const,
    int do_relu,
    int right_shift_width,
    const int* threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_pooling_fixed_forward_bmkernel");
        auto *pooling = inst->mutable_pooling();
        pooling->set_ifmap_gaddr(ifmap_gaddr);
        pooling->set_ofmap_gaddr(ofmap_gaddr);
        pooling->set_index_gaddr(index_gaddr);
        pooling->set_o_findex_gaddr(o_findex_gaddr);
        pooling->set_n(n);
        pooling->set_c(c);
        pooling->set_h(h);
        pooling->set_w(w);
        pooling->set_kh(kh);
        pooling->set_kw(kw);
        pooling->set_pad_top(pad_top);
        pooling->set_pad_bot(pad_bot);
        pooling->set_pad_left(pad_left);
        pooling->set_pad_right(pad_right);
        pooling->set_stride_h(stride_h);
        pooling->set_stride_w(stride_w);
        pooling->set_is_avg_pooling(is_avg_pooling);
        pooling->set_avg_const(avg_const);
        pooling->set_do_relu(do_relu);
        pooling->set_right_shift_width(right_shift_width);
        for (size_t i = 0; i < (size_t)1; i++)
            pooling->add_threshold_x_quantized(threshold_x_quantized[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_conv_fixed_forward_bmkernel(
    gaddr_t ga_ifmap,
    gaddr_t ga_ofmap,
    gaddr_t ga_weight,
    gaddr_t ga_bias,
    gaddr_t ga_bn_mean,
    gaddr_t ga_bn_variance,
    gaddr_t ga_scale,
    gaddr_t ga_scale_bias,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int groups,
    int output_c,
    u16 kh,
    u16 kw,
    u16 dilation_h,
    u16 dilation_w,
    u8 pad_h,
    u8 pad_w,
    u8 stride_h,
    u8 stride_w,
    int result_add,
    int do_bias,
    int do_bn,
    int do_scale,
    int do_scale_bias,
    int do_activation,
    float bn_scale,
    float bn_eps,
    int activation_method,
    float* activation_arg,
    gaddr_t activation_ga_slope,
    bool activation_channel_shared,
    int activation_gt_scale,
    int activation_gt_rshift,
    int activation_le_scale,
    int activation_le_rshift,
    int right_shift_width,
    int bn_right_shift_width,
    int scalar_right_shift_width,
    bool use_winograd)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_conv_fixed_forward_bmkernel");
        auto *conv = inst->mutable_conv();
        conv->set_ga_ifmap(ga_ifmap);
        conv->set_ga_ofmap(ga_ofmap);
        conv->set_ga_weight(ga_weight);
        conv->set_ga_bias(ga_bias);
        conv->set_ga_bn_mean(ga_bn_mean);
        conv->set_ga_bn_variance(ga_bn_variance);
        conv->set_ga_scale(ga_scale);
        conv->set_ga_scale_bias(ga_scale_bias);
        conv->set_input_n(input_n);
        conv->set_input_c(input_c);
        conv->set_input_h(input_h);
        conv->set_input_w(input_w);
        conv->set_groups(groups);
        conv->set_output_c(output_c);
        conv->set_kh(kh);
        conv->set_kw(kw);
        conv->set_dilation_h(dilation_h);
        conv->set_dilation_w(dilation_w);
        conv->set_pad_h(pad_h);
        conv->set_pad_w(pad_w);
        conv->set_stride_h(stride_h);
        conv->set_stride_w(stride_w);
        conv->set_result_add(result_add);
        conv->set_do_bias(do_bias);
        conv->set_do_bn(do_bn);
        conv->set_do_scale(do_scale);
        conv->set_do_scale_bias(do_scale_bias);
        conv->set_do_activation(do_activation);
        conv->set_bn_scale(bn_scale);
        conv->set_bn_eps(bn_eps);
        conv->set_activation_method(activation_method);
        for (size_t i = 0; i < (size_t)1; i++)
            conv->add_activation_arg(activation_arg[i]);
        conv->set_activation_ga_slope(activation_ga_slope);
        conv->set_activation_channel_shared(activation_channel_shared);
        conv->set_activation_gt_scale(activation_gt_scale);
        conv->set_activation_gt_rshift(activation_gt_rshift);
        conv->set_activation_le_scale(activation_le_scale);
        conv->set_activation_le_rshift(activation_le_rshift);
        conv->set_right_shift_width(right_shift_width);
        conv->set_bn_right_shift_width(bn_right_shift_width);
        conv->set_scalar_right_shift_width(scalar_right_shift_width);
        conv->set_use_winograd(use_winograd);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_conv_parallel_fixed_forward_bmkernel(
    gaddr_t ga_ifmap,
    gaddr_t ga_ofmap,
    gaddr_t ga_weight,
    gaddr_t ga_bias,
    gaddr_t ga_bn_mean,
    gaddr_t ga_bn_variance,
    gaddr_t ga_scale,
    gaddr_t ga_scale_bias,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int groups,
    int output_c,
    u16 kh,
    u16 kw,
    u16 dilation_h,
    u16 dilation_w,
    u8 pad_h,
    u8 pad_w,
    u8 stride_h,
    u8 stride_w,
    int result_add,
    int do_bias,
    int do_bn,
    int do_scale,
    int do_scale_bias,
    int do_activation,
    float bn_scale,
    float bn_eps,
    int activation_method,
    float* activation_arg,
    gaddr_t activation_ga_slope,
    bool activation_channel_shared,
    int activation_gt_scale,
    int activation_gt_rshift,
    int activation_le_scale,
    int activation_le_rshift,
    int right_shift_width,
    int bn_right_shift_width,
    int scale_right_shift_width,
    bool use_winograd)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_conv_parallel_fixed_forward_bmkernel");
        auto *conv_p = inst->mutable_conv_p();
        conv_p->set_ga_ifmap(ga_ifmap);
        conv_p->set_ga_ofmap(ga_ofmap);
        conv_p->set_ga_weight(ga_weight);
        conv_p->set_ga_bias(ga_bias);
        conv_p->set_ga_bn_mean(ga_bn_mean);
        conv_p->set_ga_bn_variance(ga_bn_variance);
        conv_p->set_ga_scale(ga_scale);
        conv_p->set_ga_scale_bias(ga_scale_bias);
        conv_p->set_input_n(input_n);
        conv_p->set_input_c(input_c);
        conv_p->set_input_h(input_h);
        conv_p->set_input_w(input_w);
        conv_p->set_groups(groups);
        conv_p->set_output_c(output_c);
        conv_p->set_kh(kh);
        conv_p->set_kw(kw);
        conv_p->set_dilation_h(dilation_h);
        conv_p->set_dilation_w(dilation_w);
        conv_p->set_pad_h(pad_h);
        conv_p->set_pad_w(pad_w);
        conv_p->set_stride_h(stride_h);
        conv_p->set_stride_w(stride_w);
        conv_p->set_result_add(result_add);
        conv_p->set_do_bias(do_bias);
        conv_p->set_do_bn(do_bn);
        conv_p->set_do_scale(do_scale);
        conv_p->set_do_scale_bias(do_scale_bias);
        conv_p->set_do_activation(do_activation);
        conv_p->set_bn_scale(bn_scale);
        conv_p->set_bn_eps(bn_eps);
        conv_p->set_activation_method(activation_method);
        for (size_t i = 0; i < (size_t)1; i++)
            conv_p->add_activation_arg(activation_arg[i]);
        conv_p->set_activation_ga_slope(activation_ga_slope);
        conv_p->set_activation_channel_shared(activation_channel_shared);
        conv_p->set_activation_gt_scale(activation_gt_scale);
        conv_p->set_activation_gt_rshift(activation_gt_rshift);
        conv_p->set_activation_le_scale(activation_le_scale);
        conv_p->set_activation_le_rshift(activation_le_rshift);
        conv_p->set_right_shift_width(right_shift_width);
        conv_p->set_bn_right_shift_width(bn_right_shift_width);
        conv_p->set_scale_right_shift_width(scale_right_shift_width);
        conv_p->set_use_winograd(use_winograd);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_fc_fixed_forward_bmkernel(
    gaddr_t bottom_data_gaddr,
    gaddr_t weight_data_gaddr,
    gaddr_t bias_data_gaddr,
    gaddr_t top_data_gaddr,
    int input_row_num,
    int input_col_num,
    int weight_col_num,
    int have_bias,
    int do_activation,
    int activation_method,
    gaddr_t activation_ga_slope,
    int activation_channel_shared,
    int activation_gt_scale,
    int activation_gt_rshift,
    int activation_le_scale,
    int activation_le_rshift,
    bool weight_transpose,
    int left_shift_width,
    int right_shift_width)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_fc_fixed_forward_bmkernel");
        auto *fc = inst->mutable_fc();
        fc->set_bottom_data_gaddr(bottom_data_gaddr);
        fc->set_weight_data_gaddr(weight_data_gaddr);
        fc->set_bias_data_gaddr(bias_data_gaddr);
        fc->set_top_data_gaddr(top_data_gaddr);
        fc->set_input_row_num(input_row_num);
        fc->set_input_col_num(input_col_num);
        fc->set_weight_col_num(weight_col_num);
        fc->set_have_bias(have_bias);
        fc->set_do_activation(do_activation);
        fc->set_activation_method(activation_method);
        fc->set_activation_ga_slope(activation_ga_slope);
        fc->set_activation_channel_shared(activation_channel_shared);
        fc->set_activation_gt_scale(activation_gt_scale);
        fc->set_activation_gt_rshift(activation_gt_rshift);
        fc->set_activation_le_scale(activation_le_scale);
        fc->set_activation_le_rshift(activation_le_rshift);
        fc->set_weight_transpose(weight_transpose);
        fc->set_left_shift_width(left_shift_width);
        fc->set_right_shift_width(right_shift_width);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_relu_fixed_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    float negative_slope,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_relu_fixed_forward_bmkernel");
        auto *relu = inst->mutable_relu();
        relu->set_bottom_gaddr(bottom_gaddr);
        relu->set_top_gaddr(top_gaddr);
        relu->set_negative_slope(negative_slope);
        relu->set_input_n(input_n);
        relu->set_input_c(input_c);
        relu->set_input_h(input_h);
        relu->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_leakyrelu_fixed_forward_bmkernel(
    u64 input_gaddr,
    u64 output_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int GT_right_shift_width,
    int LE_right_shift_width,
    int GT_scale,
    int LE_scale)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_leakyrelu_fixed_forward_bmkernel");
        auto *leakyrelu = inst->mutable_leakyrelu();
        leakyrelu->set_input_gaddr(input_gaddr);
        leakyrelu->set_output_gaddr(output_gaddr);
        leakyrelu->set_input_n(input_n);
        leakyrelu->set_input_c(input_c);
        leakyrelu->set_input_h(input_h);
        leakyrelu->set_input_w(input_w);
        leakyrelu->set_gt_right_shift_width(GT_right_shift_width);
        leakyrelu->set_le_right_shift_width(LE_right_shift_width);
        leakyrelu->set_gt_scale(GT_scale);
        leakyrelu->set_le_scale(LE_scale);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_prelu_fixed_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t slope_gaddr,
    gaddr_t output_gaddr,
    int channel_shared,
    int slope,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int GT_scale,
    int GT_right_shift_width,
    int LE_right_shift_width)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_prelu_fixed_forward_bmkernel");
        auto *prelu = inst->mutable_prelu();
        prelu->set_input_gaddr(input_gaddr);
        prelu->set_slope_gaddr(slope_gaddr);
        prelu->set_output_gaddr(output_gaddr);
        prelu->set_channel_shared(channel_shared);
        prelu->set_slope(slope);
        prelu->set_input_n(input_n);
        prelu->set_input_c(input_c);
        prelu->set_input_h(input_h);
        prelu->set_input_w(input_w);
        prelu->set_gt_scale(GT_scale);
        prelu->set_gt_right_shift_width(GT_right_shift_width);
        prelu->set_le_right_shift_width(LE_right_shift_width);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_batchnorm_fixed_forward_inference_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t mean_ma_gaddr,
    gaddr_t variance_ma_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int right_shift_width)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_batchnorm_fixed_forward_inference_bmkernel");
        auto *batchnorm = inst->mutable_batchnorm();
        batchnorm->set_bottom_gaddr(bottom_gaddr);
        batchnorm->set_mean_ma_gaddr(mean_ma_gaddr);
        batchnorm->set_variance_ma_gaddr(variance_ma_gaddr);
        batchnorm->set_top_gaddr(top_gaddr);
        batchnorm->set_input_n(input_n);
        batchnorm->set_input_c(input_c);
        batchnorm->set_input_h(input_h);
        batchnorm->set_input_w(input_w);
        batchnorm->set_right_shift_width(right_shift_width);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_scale_fixed_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t scale_gaddr,
    gaddr_t bias_gaddr,
    gaddr_t output_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int scale_dim,
    int inner_dim,
    int right_shift_width)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_scale_fixed_forward_bmkernel");
        auto *scale = inst->mutable_scale();
        scale->set_input_gaddr(input_gaddr);
        scale->set_scale_gaddr(scale_gaddr);
        scale->set_bias_gaddr(bias_gaddr);
        scale->set_output_gaddr(output_gaddr);
        scale->set_input_n(input_n);
        scale->set_input_c(input_c);
        scale->set_input_h(input_h);
        scale->set_input_w(input_w);
        scale->set_scale_dim(scale_dim);
        scale->set_inner_dim(inner_dim);
        scale->set_right_shift_width(right_shift_width);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_reshape_fixed_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    int output_dim_len,
    int* output_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_reshape_fixed_forward_bmkernel");
        auto *reshape = inst->mutable_reshape();
        reshape->set_input_gaddr(input_gaddr);
        reshape->set_output_gaddr(output_gaddr);
        reshape->set_output_dim_len(output_dim_len);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            reshape->add_output_dim(output_dim[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_split_fixed_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t* top_gaddr,
    int top_size,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_split_fixed_forward_bmkernel");
        auto *split = inst->mutable_split();
        split->set_bottom_gaddr(bottom_gaddr);
        for (size_t i = 0; i < (size_t)top_size; i++)
            split->add_top_gaddr(top_gaddr[i]);
        split->set_top_size(top_size);
        split->set_input_n(input_n);
        split->set_input_c(input_c);
        split->set_input_h(input_h);
        split->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_concat_fixed_forward_bmkernel(
    gaddr_t* input_gaddrs,
    gaddr_t output_gaddr,
    int* input_dims,
    int input_num,
    int concat_axis,
    int output_dim_len,
    int* output_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_concat_fixed_forward_bmkernel");
        auto *concat = inst->mutable_concat();
        for (size_t i = 0; i < (size_t)input_num; i++)
            concat->add_input_gaddrs(input_gaddrs[i]);
        concat->set_output_gaddr(output_gaddr);
        for (size_t i = 0; i < (size_t)input_num; i++)
            concat->add_input_dims(input_dims[i]);
        concat->set_input_num(input_num);
        concat->set_concat_axis(concat_axis);
        concat->set_output_dim_len(output_dim_len);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            concat->add_output_dim(output_dim[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_eltwise_fixed_forward_bmkernel(
    gaddr_t* ga_input,
    gaddr_t ga_output,
    int input_size,
    int op,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    bool do_relu,
    float relu_slope,
    int right_shift_width,
    const int* threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_eltwise_fixed_forward_bmkernel");
        auto *eltwise = inst->mutable_eltwise();
        for (size_t i = 0; i < (size_t)input_size; i++)
            eltwise->add_ga_input(ga_input[i]);
        eltwise->set_ga_output(ga_output);
        eltwise->set_input_size(input_size);
        eltwise->set_op(op);
        eltwise->set_input_n(input_n);
        eltwise->set_input_c(input_c);
        eltwise->set_input_h(input_h);
        eltwise->set_input_w(input_w);
        eltwise->set_do_relu(do_relu);
        eltwise->set_relu_slope(relu_slope);
        eltwise->set_right_shift_width(right_shift_width);
        for (size_t i = 0; i < (size_t)input_size; i++)
            eltwise->add_threshold_x_quantized(threshold_x_quantized[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_permute_fixed_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int output_n,
    int output_c,
    int output_h,
    int output_w,
    int order_n,
    int order_c,
    int order_h,
    int order_w,
    bool need_permute)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_permute_fixed_forward_bmkernel");
        auto *permute = inst->mutable_permute();
        permute->set_input_gaddr(input_gaddr);
        permute->set_output_gaddr(output_gaddr);
        permute->set_input_n(input_n);
        permute->set_input_c(input_c);
        permute->set_input_h(input_h);
        permute->set_input_w(input_w);
        permute->set_output_n(output_n);
        permute->set_output_c(output_c);
        permute->set_output_h(output_h);
        permute->set_output_w(output_w);
        permute->set_order_n(order_n);
        permute->set_order_c(order_c);
        permute->set_order_h(order_h);
        permute->set_order_w(order_w);
        permute->set_need_permute(need_permute);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_priorbox_fixed_forward_bmkernel(
    gaddr_t weight_data_gaddr,
    gaddr_t output_gaddr,
    int output_c,
    int output_h,
    int output_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_priorbox_fixed_forward_bmkernel");
        auto *priorbox = inst->mutable_priorbox();
        priorbox->set_weight_data_gaddr(weight_data_gaddr);
        priorbox->set_output_gaddr(output_gaddr);
        priorbox->set_output_c(output_c);
        priorbox->set_output_h(output_h);
        priorbox->set_output_w(output_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_lrn_fixed_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    gaddr_t sqr_lut_gaddr,
    gaddr_t power_lut_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int size,
    int sum_right_shift_width,
    int lrn_right_shift_width,
    const int* threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_lrn_fixed_forward_bmkernel");
        auto *lrn = inst->mutable_lrn();
        lrn->set_bottom_gaddr(bottom_gaddr);
        lrn->set_top_gaddr(top_gaddr);
        lrn->set_sqr_lut_gaddr(sqr_lut_gaddr);
        lrn->set_power_lut_gaddr(power_lut_gaddr);
        lrn->set_input_n(input_n);
        lrn->set_input_c(input_c);
        lrn->set_input_h(input_h);
        lrn->set_input_w(input_w);
        lrn->set_size(size);
        lrn->set_sum_right_shift_width(sum_right_shift_width);
        lrn->set_lrn_right_shift_width(lrn_right_shift_width);
        for (size_t i = 0; i < (size_t)2; i++)
            lrn->add_threshold_x_quantized(threshold_x_quantized[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_upsample_fixed_bmkernel(
    gaddr_t ga_ifmap,
    gaddr_t ga_ofmap,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int size)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_upsample_fixed_bmkernel");
        auto *upsample = inst->mutable_upsample();
        upsample->set_ga_ifmap(ga_ifmap);
        upsample->set_ga_ofmap(ga_ofmap);
        upsample->set_input_n(input_n);
        upsample->set_input_c(input_c);
        upsample->set_input_h(input_h);
        upsample->set_input_w(input_w);
        upsample->set_size(size);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_normalize_fixed_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    bool has_scale_blob,
    gaddr_t scale_gaddr,
    gaddr_t sqr_lut_gaddr,
    gaddr_t sqrt_lut_gaddr,
    bool across_spatial,
    bool channel_shared,
    int* scales,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int sum_right_shift_width,
    int norma_right_shift_width,
    int scale_right_shift_width,
    const int* threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_normalize_fixed_forward_bmkernel");
        auto *normalize = inst->mutable_normalize();
        normalize->set_input_gaddr(input_gaddr);
        normalize->set_output_gaddr(output_gaddr);
        normalize->set_has_scale_blob(has_scale_blob);
        normalize->set_scale_gaddr(scale_gaddr);
        normalize->set_sqr_lut_gaddr(sqr_lut_gaddr);
        normalize->set_sqrt_lut_gaddr(sqrt_lut_gaddr);
        normalize->set_across_spatial(across_spatial);
        normalize->set_channel_shared(channel_shared);
        for (size_t i = 0; i < (size_t)0; i++)
            normalize->add_scales(scales[i]);
        normalize->set_input_n(input_n);
        normalize->set_input_c(input_c);
        normalize->set_input_h(input_h);
        normalize->set_input_w(input_w);
        normalize->set_sum_right_shift_width(sum_right_shift_width);
        normalize->set_norma_right_shift_width(norma_right_shift_width);
        normalize->set_scale_right_shift_width(scale_right_shift_width);
        for (size_t i = 0; i < (size_t)2; i++)
            normalize->add_threshold_x_quantized(threshold_x_quantized[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_conv_forward_bmkernel(
    laddr_t la_ifmap,
    laddr_t la_ofmap,
    laddr_t la_weight,
    laddr_t la_bias,
    u32* group_weight,
    u32* group_bias,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int group,
    int output_c,
    int output_h,
    int output_w,
    u32 kh,
    u32 kw,
    u32 dh,
    u32 dw,
    u32 pad_h_top,
    u32 pad_h_bottom,
    u32 pad_w_left,
    u32 pad_w_right,
    u32 stride_h,
    u32 stride_w,
    u32 result_add,
    u32 ctrl,
    int rshift,
    bool do_bias,
    bool use_winograd,
    bool do_relu)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_conv_forward_bmkernel");
        auto *bm_tl_conv = inst->mutable_bm_tl_conv();
        bm_tl_conv->set_la_ifmap(la_ifmap);
        bm_tl_conv->set_la_ofmap(la_ofmap);
        bm_tl_conv->set_la_weight(la_weight);
        bm_tl_conv->set_la_bias(la_bias);
        for (size_t i = 0; i < (size_t)group; i++)
            bm_tl_conv->add_group_weight(group_weight[i]);
        for (size_t i = 0; i < (size_t)group; i++)
            bm_tl_conv->add_group_bias(group_bias[i]);
        bm_tl_conv->set_input_n(input_n);
        bm_tl_conv->set_input_c(input_c);
        bm_tl_conv->set_input_h(input_h);
        bm_tl_conv->set_input_w(input_w);
        bm_tl_conv->set_group(group);
        bm_tl_conv->set_output_c(output_c);
        bm_tl_conv->set_output_h(output_h);
        bm_tl_conv->set_output_w(output_w);
        bm_tl_conv->set_kh(kh);
        bm_tl_conv->set_kw(kw);
        bm_tl_conv->set_dh(dh);
        bm_tl_conv->set_dw(dw);
        bm_tl_conv->set_pad_h_top(pad_h_top);
        bm_tl_conv->set_pad_h_bottom(pad_h_bottom);
        bm_tl_conv->set_pad_w_left(pad_w_left);
        bm_tl_conv->set_pad_w_right(pad_w_right);
        bm_tl_conv->set_stride_h(stride_h);
        bm_tl_conv->set_stride_w(stride_w);
        bm_tl_conv->set_result_add(result_add);
        bm_tl_conv->set_ctrl(ctrl);
        bm_tl_conv->set_rshift(rshift);
        bm_tl_conv->set_do_bias(do_bias);
        bm_tl_conv->set_use_winograd(use_winograd);
        bm_tl_conv->set_do_relu(do_relu);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_activation_forward_bmkernel(
    laddr_t input_laddr,
    laddr_t output_laddr,
    laddr_t weight_laddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int gt_rshift,
    int le_rshift,
    int gt_scale,
    int le_scale,
    int gt_right_shift_width,
    int le_right_shift_width,
    int activation_arg_len,
    float* activation_arg,
    bool channel_shared,
    bmnet::bm1880::Inst::ActivationMethod activation_type)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_activation_forward_bmkernel");
        auto *tl_activation = inst->mutable_tl_activation();
        tl_activation->set_input_laddr(input_laddr);
        tl_activation->set_output_laddr(output_laddr);
        tl_activation->set_weight_laddr(weight_laddr);
        tl_activation->set_input_n(input_n);
        tl_activation->set_input_c(input_c);
        tl_activation->set_input_h(input_h);
        tl_activation->set_input_w(input_w);
        tl_activation->set_gt_rshift(gt_rshift);
        tl_activation->set_le_rshift(le_rshift);
        tl_activation->set_gt_scale(gt_scale);
        tl_activation->set_le_scale(le_scale);
        tl_activation->set_gt_right_shift_width(gt_right_shift_width);
        tl_activation->set_le_right_shift_width(le_right_shift_width);
        tl_activation->set_activation_arg_len(activation_arg_len);
        for (size_t i = 0; i < (size_t)activation_arg_len; i++)
            tl_activation->add_activation_arg(activation_arg[i]);
        tl_activation->set_channel_shared(channel_shared);
        tl_activation->set_activation_type((bmnet::bm1880::Inst::ActivationMethod)activation_type);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_scale_forward_bmkernel(
    laddr_t input_laddr,
    laddr_t output_laddr,
    laddr_t scale_laddr,
    laddr_t bias_laddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int scale_dim,
    int right_shift_width,
    bool do_bias,
    bool do_relu,
    float relu_slope)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_scale_forward_bmkernel");
        auto *tl_csale = inst->mutable_tl_csale();
        tl_csale->set_input_laddr(input_laddr);
        tl_csale->set_output_laddr(output_laddr);
        tl_csale->set_scale_laddr(scale_laddr);
        tl_csale->set_bias_laddr(bias_laddr);
        tl_csale->set_input_n(input_n);
        tl_csale->set_input_c(input_c);
        tl_csale->set_input_h(input_h);
        tl_csale->set_input_w(input_w);
        tl_csale->set_scale_dim(scale_dim);
        tl_csale->set_right_shift_width(right_shift_width);
        tl_csale->set_do_bias(do_bias);
        tl_csale->set_do_relu(do_relu);
        tl_csale->set_relu_slope(relu_slope);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_batchnorm_forward_bmkernel(
    laddr_t bottom_laddr,
    laddr_t top_laddr,
    laddr_t mean_laddr,
    laddr_t variance_laddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int right_shift_width,
    bool do_relu)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_batchnorm_forward_bmkernel");
        auto *tl_bn = inst->mutable_tl_bn();
        tl_bn->set_bottom_laddr(bottom_laddr);
        tl_bn->set_top_laddr(top_laddr);
        tl_bn->set_mean_laddr(mean_laddr);
        tl_bn->set_variance_laddr(variance_laddr);
        tl_bn->set_input_n(input_n);
        tl_bn->set_input_c(input_c);
        tl_bn->set_input_h(input_h);
        tl_bn->set_input_w(input_w);
        tl_bn->set_right_shift_width(right_shift_width);
        tl_bn->set_do_relu(do_relu);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_eltwise_forward_bmkernel(
    laddr_t* la_input,
    laddr_t la_output,
    laddr_t la_working,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int input_size,
    int right_shift_width,
    int op,
    const int* threshold_x_quantized,
    bool use_default_coeff,
    bool do_relu,
    float relu_slope)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_eltwise_forward_bmkernel");
        auto *tl_eltwise = inst->mutable_tl_eltwise();
        for (size_t i = 0; i < (size_t)input_size; i++)
            tl_eltwise->add_la_input(la_input[i]);
        tl_eltwise->set_la_output(la_output);
        tl_eltwise->set_la_working(la_working);
        tl_eltwise->set_input_n(input_n);
        tl_eltwise->set_input_c(input_c);
        tl_eltwise->set_input_h(input_h);
        tl_eltwise->set_input_w(input_w);
        tl_eltwise->set_input_size(input_size);
        tl_eltwise->set_right_shift_width(right_shift_width);
        tl_eltwise->set_op(op);
        for (size_t i = 0; i < (size_t)input_size; i++)
            tl_eltwise->add_threshold_x_quantized(threshold_x_quantized[i]);
        tl_eltwise->set_use_default_coeff(use_default_coeff);
        tl_eltwise->set_do_relu(do_relu);
        tl_eltwise->set_relu_slope(relu_slope);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_lrn_forward_bmkernel(
    laddr_t bottom_laddr,
    laddr_t top_laddr,
    laddr_t sqr_lut_laddr,
    laddr_t power_lut_laddr,
    laddr_t working_laddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int size,
    int sum_right_shift_width,
    int lrn_right_shift_width,
    const int* threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_lrn_forward_bmkernel");
        auto *tl_lrn = inst->mutable_tl_lrn();
        tl_lrn->set_bottom_laddr(bottom_laddr);
        tl_lrn->set_top_laddr(top_laddr);
        tl_lrn->set_sqr_lut_laddr(sqr_lut_laddr);
        tl_lrn->set_power_lut_laddr(power_lut_laddr);
        tl_lrn->set_working_laddr(working_laddr);
        tl_lrn->set_input_n(input_n);
        tl_lrn->set_input_c(input_c);
        tl_lrn->set_input_h(input_h);
        tl_lrn->set_input_w(input_w);
        tl_lrn->set_size(size);
        tl_lrn->set_sum_right_shift_width(sum_right_shift_width);
        tl_lrn->set_lrn_right_shift_width(lrn_right_shift_width);
        for (size_t i = 0; i < (size_t)2; i++)
            tl_lrn->add_threshold_x_quantized(threshold_x_quantized[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_pooling_forward_bmkernel(
    laddr_t ifmap_laddr,
    laddr_t ofmap_laddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int output_n,
    int output_c,
    int output_h,
    int output_w,
    u32 kh,
    u32 kw,
    u32 stride_h,
    u32 stride_w,
    u32 pad_h_top,
    u32 pad_h_bottom,
    u32 pad_w_left,
    u32 pad_w_right,
    bool is_avg_pooling,
    int right_shift_width,
    int threshold_x_quantized)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_pooling_forward_bmkernel");
        auto *tl_pool = inst->mutable_tl_pool();
        tl_pool->set_ifmap_laddr(ifmap_laddr);
        tl_pool->set_ofmap_laddr(ofmap_laddr);
        tl_pool->set_input_n(input_n);
        tl_pool->set_input_c(input_c);
        tl_pool->set_input_h(input_h);
        tl_pool->set_input_w(input_w);
        tl_pool->set_output_n(output_n);
        tl_pool->set_output_c(output_c);
        tl_pool->set_output_h(output_h);
        tl_pool->set_output_w(output_w);
        tl_pool->set_kh(kh);
        tl_pool->set_kw(kw);
        tl_pool->set_stride_h(stride_h);
        tl_pool->set_stride_w(stride_w);
        tl_pool->set_pad_h_top(pad_h_top);
        tl_pool->set_pad_h_bottom(pad_h_bottom);
        tl_pool->set_pad_w_left(pad_w_left);
        tl_pool->set_pad_w_right(pad_w_right);
        tl_pool->set_is_avg_pooling(is_avg_pooling);
        tl_pool->set_right_shift_width(right_shift_width);
        tl_pool->set_threshold_x_quantized(threshold_x_quantized);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tl_upsample_forward_bmkernel(
    laddr_t la_ifmap,
    laddr_t la_ofmap,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int output_h,
    int output_w,
    int size)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1880::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tl_upsample_forward_bmkernel");
        auto *tl_upsample = inst->mutable_tl_upsample();
        tl_upsample->set_la_ifmap(la_ifmap);
        tl_upsample->set_la_ofmap(la_ofmap);
        tl_upsample->set_input_n(input_n);
        tl_upsample->set_input_c(input_c);
        tl_upsample->set_input_h(input_h);
        tl_upsample->set_input_w(input_w);
        tl_upsample->set_output_h(output_h);
        tl_upsample->set_output_w(output_w);
        tl_upsample->set_size(size);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
// clang-format on

} // namespace bmnet_asm
} // namespace bmnet
#endif /* BM188X_BMKERNEL_API_H */