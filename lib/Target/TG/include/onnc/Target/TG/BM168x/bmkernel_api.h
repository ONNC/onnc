#ifndef BM168X_BMKERNEL_API_H
#define BM168X_BMKERNEL_API_H

#include <onnc/Target/TG/BM168x/asm/bm168x_asm.pb.h>
#include <onnc/Target/TG/bmkernel_api_base.h>

namespace bmnet {

namespace bmnet_asm {

// clang-format off
inline void bmnet_pooling_forward_bmkernel(
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
    const bool ceil_mode)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_pooling_forward_bmkernel");
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
        pooling->set_ceil_mode(ceil_mode);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_conv_forward_bmkernel(
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
    bool activation_channel_shared)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_conv_forward_bmkernel");
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
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_threshold_forward_bmkernel(
    gaddr_t input_offset,
    gaddr_t output_offset,
    float threshold,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_threshold_forward_bmkernel");
        auto *bmnet_threshold_forward_bmkernel_short = inst->mutable_bmnet_threshold_forward_bmkernel_short();
        bmnet_threshold_forward_bmkernel_short->set_input_offset(input_offset);
        bmnet_threshold_forward_bmkernel_short->set_output_offset(output_offset);
        bmnet_threshold_forward_bmkernel_short->set_threshold(threshold);
        bmnet_threshold_forward_bmkernel_short->set_input_n(input_n);
        bmnet_threshold_forward_bmkernel_short->set_input_c(input_c);
        bmnet_threshold_forward_bmkernel_short->set_input_h(input_h);
        bmnet_threshold_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_exp_forward_bmkernel(
    gaddr_t input_offset,
    gaddr_t output_offset,
    float base,
    float input_scale,
    float input_shift,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_exp_forward_bmkernel");
        auto *bmnet_exp_forward_bmkernel_short = inst->mutable_bmnet_exp_forward_bmkernel_short();
        bmnet_exp_forward_bmkernel_short->set_input_offset(input_offset);
        bmnet_exp_forward_bmkernel_short->set_output_offset(output_offset);
        bmnet_exp_forward_bmkernel_short->set_base(base);
        bmnet_exp_forward_bmkernel_short->set_input_scale(input_scale);
        bmnet_exp_forward_bmkernel_short->set_input_shift(input_shift);
        bmnet_exp_forward_bmkernel_short->set_input_n(input_n);
        bmnet_exp_forward_bmkernel_short->set_input_c(input_c);
        bmnet_exp_forward_bmkernel_short->set_input_h(input_h);
        bmnet_exp_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_power_forward_bmkernel(
    gaddr_t input_offset,
    gaddr_t output_offset,
    float power_,
    float scale_,
    float shift_,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_power_forward_bmkernel");
        auto *bmnet_power_forward_bmkernel_short = inst->mutable_bmnet_power_forward_bmkernel_short();
        bmnet_power_forward_bmkernel_short->set_input_offset(input_offset);
        bmnet_power_forward_bmkernel_short->set_output_offset(output_offset);
        bmnet_power_forward_bmkernel_short->set_power_(power_);
        bmnet_power_forward_bmkernel_short->set_scale_(scale_);
        bmnet_power_forward_bmkernel_short->set_shift_(shift_);
        bmnet_power_forward_bmkernel_short->set_input_n(input_n);
        bmnet_power_forward_bmkernel_short->set_input_c(input_c);
        bmnet_power_forward_bmkernel_short->set_input_h(input_h);
        bmnet_power_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_absval_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_absval_forward_bmkernel");
        auto *bmnet_absval_forward_bmkernel_short = inst->mutable_bmnet_absval_forward_bmkernel_short();
        bmnet_absval_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_absval_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_absval_forward_bmkernel_short->set_input_n(input_n);
        bmnet_absval_forward_bmkernel_short->set_input_c(input_c);
        bmnet_absval_forward_bmkernel_short->set_input_h(input_h);
        bmnet_absval_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_sigmoid_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_sigmoid_forward_bmkernel");
        auto *bmnet_sigmoid_forward_bmkernel_short = inst->mutable_bmnet_sigmoid_forward_bmkernel_short();
        bmnet_sigmoid_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_sigmoid_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_sigmoid_forward_bmkernel_short->set_input_n(input_n);
        bmnet_sigmoid_forward_bmkernel_short->set_input_c(input_c);
        bmnet_sigmoid_forward_bmkernel_short->set_input_h(input_h);
        bmnet_sigmoid_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_log_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    float scale,
    float shift,
    float base,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_log_forward_bmkernel");
        auto *bmnet_log_forward_bmkernel_short = inst->mutable_bmnet_log_forward_bmkernel_short();
        bmnet_log_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_log_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_log_forward_bmkernel_short->set_scale(scale);
        bmnet_log_forward_bmkernel_short->set_shift(shift);
        bmnet_log_forward_bmkernel_short->set_base(base);
        bmnet_log_forward_bmkernel_short->set_input_n(input_n);
        bmnet_log_forward_bmkernel_short->set_input_c(input_c);
        bmnet_log_forward_bmkernel_short->set_input_h(input_h);
        bmnet_log_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_fc_forward_bmkernel(
    gaddr_t bottom_data_gaddr,
    gaddr_t weight_data_gaddr,
    gaddr_t bias_data_gaddr,
    gaddr_t top_data_gaddr,
    int input_row_num,
    int input_col_num,
    int weight_col_num,
    int have_bias,
    int using_relu,
    bool weight_transpose)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_fc_forward_bmkernel");
        auto *bmnet_fc_forward_bmkernel_short = inst->mutable_bmnet_fc_forward_bmkernel_short();
        bmnet_fc_forward_bmkernel_short->set_bottom_data_gaddr(bottom_data_gaddr);
        bmnet_fc_forward_bmkernel_short->set_weight_data_gaddr(weight_data_gaddr);
        bmnet_fc_forward_bmkernel_short->set_bias_data_gaddr(bias_data_gaddr);
        bmnet_fc_forward_bmkernel_short->set_top_data_gaddr(top_data_gaddr);
        bmnet_fc_forward_bmkernel_short->set_input_row_num(input_row_num);
        bmnet_fc_forward_bmkernel_short->set_input_col_num(input_col_num);
        bmnet_fc_forward_bmkernel_short->set_weight_col_num(weight_col_num);
        bmnet_fc_forward_bmkernel_short->set_have_bias(have_bias);
        bmnet_fc_forward_bmkernel_short->set_using_relu(using_relu);
        bmnet_fc_forward_bmkernel_short->set_weight_transpose(weight_transpose);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_bias_forward_bmkernel(
    gaddr_t bottom_data_gaddr,
    gaddr_t bias_data_gaddr,
    gaddr_t top_data_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_bias_forward_bmkernel");
        auto *bmnet_bias_forward_bmkernel_short = inst->mutable_bmnet_bias_forward_bmkernel_short();
        bmnet_bias_forward_bmkernel_short->set_bottom_data_gaddr(bottom_data_gaddr);
        bmnet_bias_forward_bmkernel_short->set_bias_data_gaddr(bias_data_gaddr);
        bmnet_bias_forward_bmkernel_short->set_top_data_gaddr(top_data_gaddr);
        bmnet_bias_forward_bmkernel_short->set_input_n(input_n);
        bmnet_bias_forward_bmkernel_short->set_input_c(input_c);
        bmnet_bias_forward_bmkernel_short->set_input_h(input_h);
        bmnet_bias_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_elu_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    float alpha,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_elu_forward_bmkernel");
        auto *bmnet_elu_forward_bmkernel_short = inst->mutable_bmnet_elu_forward_bmkernel_short();
        bmnet_elu_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_elu_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_elu_forward_bmkernel_short->set_alpha(alpha);
        bmnet_elu_forward_bmkernel_short->set_input_n(input_n);
        bmnet_elu_forward_bmkernel_short->set_input_c(input_c);
        bmnet_elu_forward_bmkernel_short->set_input_h(input_h);
        bmnet_elu_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tanh_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tanh_forward_bmkernel");
        auto *bmnet_tanh_forward_bmkernel_short = inst->mutable_bmnet_tanh_forward_bmkernel_short();
        bmnet_tanh_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_tanh_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_tanh_forward_bmkernel_short->set_input_n(input_n);
        bmnet_tanh_forward_bmkernel_short->set_input_c(input_c);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_lstm_unit_forward_bmkernel(
    gaddr_t X_i_offset,
    gaddr_t X_f_offset,
    gaddr_t X_o_offset,
    gaddr_t X_g_offset,
    gaddr_t C_prev_offset,
    gaddr_t cont_expand_offset,
    gaddr_t C_offset,
    gaddr_t H_offset,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_lstm_unit_forward_bmkernel");
        auto *bmnet_lstm_unit_forward_bmkernel_short = inst->mutable_bmnet_lstm_unit_forward_bmkernel_short();
        bmnet_lstm_unit_forward_bmkernel_short->set_x_i_offset(X_i_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_x_f_offset(X_f_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_x_o_offset(X_o_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_x_g_offset(X_g_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_c_prev_offset(C_prev_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_cont_expand_offset(cont_expand_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_c_offset(C_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_h_offset(H_offset);
        bmnet_lstm_unit_forward_bmkernel_short->set_input_n(input_n);
        bmnet_lstm_unit_forward_bmkernel_short->set_input_c(input_c);
        bmnet_lstm_unit_forward_bmkernel_short->set_input_h(input_h);
        bmnet_lstm_unit_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_lrn_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    float alpha,
    int size,
    float beta,
    float k)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_lrn_forward_bmkernel");
        auto *bmnet_lrn_forward_bmkernel_short = inst->mutable_bmnet_lrn_forward_bmkernel_short();
        bmnet_lrn_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_lrn_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_lrn_forward_bmkernel_short->set_input_n(input_n);
        bmnet_lrn_forward_bmkernel_short->set_input_c(input_c);
        bmnet_lrn_forward_bmkernel_short->set_input_h(input_h);
        bmnet_lrn_forward_bmkernel_short->set_input_w(input_w);
        bmnet_lrn_forward_bmkernel_short->set_alpha(alpha);
        bmnet_lrn_forward_bmkernel_short->set_size(size);
        bmnet_lrn_forward_bmkernel_short->set_beta(beta);
        bmnet_lrn_forward_bmkernel_short->set_k(k);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_bnll_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_bnll_forward_bmkernel");
        auto *bmnet_bnll_forward_bmkernel_short = inst->mutable_bmnet_bnll_forward_bmkernel_short();
        bmnet_bnll_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_bnll_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_bnll_forward_bmkernel_short->set_input_n(input_n);
        bmnet_bnll_forward_bmkernel_short->set_input_c(input_c);
        bmnet_bnll_forward_bmkernel_short->set_input_h(input_h);
        bmnet_bnll_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_crop_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    int* input_dim,
    int* output_dim,
    int* offsets,
    int output_dim_len)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_crop_forward_bmkernel");
        auto *bmnet_crop_forward_bmkernel_short = inst->mutable_bmnet_crop_forward_bmkernel_short();
        bmnet_crop_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_crop_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            bmnet_crop_forward_bmkernel_short->add_input_dim(input_dim[i]);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            bmnet_crop_forward_bmkernel_short->add_output_dim(output_dim[i]);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            bmnet_crop_forward_bmkernel_short->add_offsets(offsets[i]);
        bmnet_crop_forward_bmkernel_short->set_output_dim_len(output_dim_len);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_slice_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int outer_dim,
    int inner_dim,
    int input_slice_dim,
    int input_slice_offset,
    int output_slice_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_slice_forward_bmkernel");
        auto *bmnet_slice_forward_bmkernel_short = inst->mutable_bmnet_slice_forward_bmkernel_short();
        bmnet_slice_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_slice_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_slice_forward_bmkernel_short->set_input_n(input_n);
        bmnet_slice_forward_bmkernel_short->set_outer_dim(outer_dim);
        bmnet_slice_forward_bmkernel_short->set_inner_dim(inner_dim);
        bmnet_slice_forward_bmkernel_short->set_input_slice_dim(input_slice_dim);
        bmnet_slice_forward_bmkernel_short->set_input_slice_offset(input_slice_offset);
        bmnet_slice_forward_bmkernel_short->set_output_slice_dim(output_slice_dim);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_eltwise_forward_bmkernel(
    gaddr_t* ga_input,
    gaddr_t ga_output,
    int input_size,
    int op,
    float* coeffs,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    bool do_relu,
    float relu_slope)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_eltwise_forward_bmkernel");
        auto *bmnet_eltwise_forward_bmkernel_short = inst->mutable_bmnet_eltwise_forward_bmkernel_short();
        for (size_t i = 0; i < (size_t)input_size; i++)
            bmnet_eltwise_forward_bmkernel_short->add_ga_input(ga_input[i]);
        bmnet_eltwise_forward_bmkernel_short->set_ga_output(ga_output);
        bmnet_eltwise_forward_bmkernel_short->set_input_size(input_size);
        bmnet_eltwise_forward_bmkernel_short->set_op(op);
        for (size_t i = 0; i < (size_t)input_size; i++)
            bmnet_eltwise_forward_bmkernel_short->add_coeffs(coeffs[i]);
        bmnet_eltwise_forward_bmkernel_short->set_input_n(input_n);
        bmnet_eltwise_forward_bmkernel_short->set_input_c(input_c);
        bmnet_eltwise_forward_bmkernel_short->set_input_h(input_h);
        bmnet_eltwise_forward_bmkernel_short->set_input_w(input_w);
        bmnet_eltwise_forward_bmkernel_short->set_do_relu(do_relu);
        bmnet_eltwise_forward_bmkernel_short->set_relu_slope(relu_slope);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_tile_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int outer_dim,
    int inner_dim,
    int tiles)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_tile_forward_bmkernel");
        auto *bmnet_tile_forward_bmkernel_short = inst->mutable_bmnet_tile_forward_bmkernel_short();
        bmnet_tile_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_tile_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_tile_forward_bmkernel_short->set_input_n(input_n);
        bmnet_tile_forward_bmkernel_short->set_outer_dim(outer_dim);
        bmnet_tile_forward_bmkernel_short->set_inner_dim(inner_dim);
        bmnet_tile_forward_bmkernel_short->set_tiles(tiles);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_relu_forward_bmkernel(
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
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_relu_forward_bmkernel");
        auto *bmnet_relu_forward_bmkernel_short = inst->mutable_bmnet_relu_forward_bmkernel_short();
        bmnet_relu_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_relu_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_relu_forward_bmkernel_short->set_negative_slope(negative_slope);
        bmnet_relu_forward_bmkernel_short->set_input_n(input_n);
        bmnet_relu_forward_bmkernel_short->set_input_c(input_c);
        bmnet_relu_forward_bmkernel_short->set_input_h(input_h);
        bmnet_relu_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_batchnorm_forward_inference_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t mean_ma_gaddr,
    gaddr_t variance_ma_gaddr,
    float scale_ma,
    gaddr_t variance_gaddr,
    gaddr_t top_gaddr,
    float eps,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_batchnorm_forward_inference_bmkernel");
        auto *bmnet_batchnorm_forward_inference_bmkernel_short = inst->mutable_bmnet_batchnorm_forward_inference_bmkernel_short();
        bmnet_batchnorm_forward_inference_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_mean_ma_gaddr(mean_ma_gaddr);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_variance_ma_gaddr(variance_ma_gaddr);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_scale_ma(scale_ma);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_variance_gaddr(variance_gaddr);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_eps(eps);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_input_n(input_n);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_input_c(input_c);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_input_h(input_h);
        bmnet_batchnorm_forward_inference_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_softmax_forward_bmkernel(
    gaddr_t bottom_gaddr,
    gaddr_t top_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_softmax_forward_bmkernel");
        auto *bmnet_softmax_forward_bmkernel_short = inst->mutable_bmnet_softmax_forward_bmkernel_short();
        bmnet_softmax_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        bmnet_softmax_forward_bmkernel_short->set_top_gaddr(top_gaddr);
        bmnet_softmax_forward_bmkernel_short->set_input_n(input_n);
        bmnet_softmax_forward_bmkernel_short->set_input_c(input_c);
        bmnet_softmax_forward_bmkernel_short->set_input_h(input_h);
        bmnet_softmax_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_scale_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t scale_gaddr,
    gaddr_t bias_gaddr,
    gaddr_t output_gaddr,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int scale_dim,
    int inner_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_scale_forward_bmkernel");
        auto *bmnet_scale_forward_bmkernel_short = inst->mutable_bmnet_scale_forward_bmkernel_short();
        bmnet_scale_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_scale_forward_bmkernel_short->set_scale_gaddr(scale_gaddr);
        bmnet_scale_forward_bmkernel_short->set_bias_gaddr(bias_gaddr);
        bmnet_scale_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_scale_forward_bmkernel_short->set_input_n(input_n);
        bmnet_scale_forward_bmkernel_short->set_input_c(input_c);
        bmnet_scale_forward_bmkernel_short->set_input_h(input_h);
        bmnet_scale_forward_bmkernel_short->set_input_w(input_w);
        bmnet_scale_forward_bmkernel_short->set_scale_dim(scale_dim);
        bmnet_scale_forward_bmkernel_short->set_inner_dim(inner_dim);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_mvn_forward_bmkernel(
    gaddr_t mean_offset,
    gaddr_t temp_offset,
    gaddr_t input_offset,
    gaddr_t variance_offset,
    gaddr_t output_offset,
    int normalize_variance,
    float eps_,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_mvn_forward_bmkernel");
        auto *bmnet_mvn_forward_bmkernel_short = inst->mutable_bmnet_mvn_forward_bmkernel_short();
        bmnet_mvn_forward_bmkernel_short->set_mean_offset(mean_offset);
        bmnet_mvn_forward_bmkernel_short->set_temp_offset(temp_offset);
        bmnet_mvn_forward_bmkernel_short->set_input_offset(input_offset);
        bmnet_mvn_forward_bmkernel_short->set_variance_offset(variance_offset);
        bmnet_mvn_forward_bmkernel_short->set_output_offset(output_offset);
        bmnet_mvn_forward_bmkernel_short->set_normalize_variance(normalize_variance);
        bmnet_mvn_forward_bmkernel_short->set_eps_(eps_);
        bmnet_mvn_forward_bmkernel_short->set_input_n(input_n);
        bmnet_mvn_forward_bmkernel_short->set_input_c(input_c);
        bmnet_mvn_forward_bmkernel_short->set_input_h(input_h);
        bmnet_mvn_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_reduction_forward_bmkernel(
    gaddr_t input_offset,
    gaddr_t output_offset,
    int op_,
    float coeff_,
    int input_c,
    int input_h)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_reduction_forward_bmkernel");
        auto *bmnet_reduction_forward_bmkernel_short = inst->mutable_bmnet_reduction_forward_bmkernel_short();
        bmnet_reduction_forward_bmkernel_short->set_input_offset(input_offset);
        bmnet_reduction_forward_bmkernel_short->set_output_offset(output_offset);
        bmnet_reduction_forward_bmkernel_short->set_op_(op_);
        bmnet_reduction_forward_bmkernel_short->set_coeff_(coeff_);
        bmnet_reduction_forward_bmkernel_short->set_input_c(input_c);
        bmnet_reduction_forward_bmkernel_short->set_input_h(input_h);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_prelu_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t slope_gaddr,
    gaddr_t output_gaddr,
    int channel_shared,
    float slope,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_prelu_forward_bmkernel");
        auto *bmnet_prelu_forward_bmkernel_short = inst->mutable_bmnet_prelu_forward_bmkernel_short();
        bmnet_prelu_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_prelu_forward_bmkernel_short->set_slope_gaddr(slope_gaddr);
        bmnet_prelu_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_prelu_forward_bmkernel_short->set_channel_shared(channel_shared);
        bmnet_prelu_forward_bmkernel_short->set_slope(slope);
        bmnet_prelu_forward_bmkernel_short->set_input_n(input_n);
        bmnet_prelu_forward_bmkernel_short->set_input_c(input_c);
        bmnet_prelu_forward_bmkernel_short->set_input_h(input_h);
        bmnet_prelu_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_split_forward_bmkernel(
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
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_split_forward_bmkernel");
        auto *bmnet_split_forward_bmkernel_short = inst->mutable_bmnet_split_forward_bmkernel_short();
        bmnet_split_forward_bmkernel_short->set_bottom_gaddr(bottom_gaddr);
        for (size_t i = 0; i < (size_t)top_size; i++)
            bmnet_split_forward_bmkernel_short->add_top_gaddr(top_gaddr[i]);
        bmnet_split_forward_bmkernel_short->set_top_size(top_size);
        bmnet_split_forward_bmkernel_short->set_input_n(input_n);
        bmnet_split_forward_bmkernel_short->set_input_c(input_c);
        bmnet_split_forward_bmkernel_short->set_input_h(input_h);
        bmnet_split_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_concat_forward_bmkernel(
    gaddr_t* input_gaddrs,
    gaddr_t* output_gaddrs,
    int* input_dims,
    int input_num,
    int output_dim_len,
    int* output_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_concat_forward_bmkernel");
        auto *bmnet_concat_forward_bmkernel_short = inst->mutable_bmnet_concat_forward_bmkernel_short();
        for (size_t i = 0; i < (size_t)input_num; i++)
            bmnet_concat_forward_bmkernel_short->add_input_gaddrs(input_gaddrs[i]);
        for (size_t i = 0; i < (size_t)input_num; i++)
            bmnet_concat_forward_bmkernel_short->add_output_gaddrs(output_gaddrs[i]);
        for (size_t i = 0; i < (size_t)input_num; i++)
            bmnet_concat_forward_bmkernel_short->add_input_dims(input_dims[i]);
        bmnet_concat_forward_bmkernel_short->set_input_num(input_num);
        bmnet_concat_forward_bmkernel_short->set_output_dim_len(output_dim_len);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            bmnet_concat_forward_bmkernel_short->add_output_dim(output_dim[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_upsample_bmkernel(
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
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_upsample_bmkernel");
        auto *bmnet_upsample_bmkernel_short = inst->mutable_bmnet_upsample_bmkernel_short();
        bmnet_upsample_bmkernel_short->set_ga_ifmap(ga_ifmap);
        bmnet_upsample_bmkernel_short->set_ga_ofmap(ga_ofmap);
        bmnet_upsample_bmkernel_short->set_input_n(input_n);
        bmnet_upsample_bmkernel_short->set_input_c(input_c);
        bmnet_upsample_bmkernel_short->set_input_h(input_h);
        bmnet_upsample_bmkernel_short->set_input_w(input_w);
        bmnet_upsample_bmkernel_short->set_size(size);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_reshape_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    int output_dim_len,
    int* output_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_reshape_forward_bmkernel");
        auto *bmnet_reshape_forward_bmkernel_short = inst->mutable_bmnet_reshape_forward_bmkernel_short();
        bmnet_reshape_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_reshape_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_reshape_forward_bmkernel_short->set_output_dim_len(output_dim_len);
        for (size_t i = 0; i < (size_t)output_dim_len; i++)
            bmnet_reshape_forward_bmkernel_short->add_output_dim(output_dim[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_permute_forward_bmkernel(
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
    bool need_permute_)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_permute_forward_bmkernel");
        auto *bmnet_permute_forward_bmkernel_short = inst->mutable_bmnet_permute_forward_bmkernel_short();
        bmnet_permute_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_permute_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_permute_forward_bmkernel_short->set_input_n(input_n);
        bmnet_permute_forward_bmkernel_short->set_input_c(input_c);
        bmnet_permute_forward_bmkernel_short->set_input_h(input_h);
        bmnet_permute_forward_bmkernel_short->set_input_w(input_w);
        bmnet_permute_forward_bmkernel_short->set_output_n(output_n);
        bmnet_permute_forward_bmkernel_short->set_output_c(output_c);
        bmnet_permute_forward_bmkernel_short->set_output_h(output_h);
        bmnet_permute_forward_bmkernel_short->set_output_w(output_w);
        bmnet_permute_forward_bmkernel_short->set_order_n(order_n);
        bmnet_permute_forward_bmkernel_short->set_order_c(order_c);
        bmnet_permute_forward_bmkernel_short->set_order_h(order_h);
        bmnet_permute_forward_bmkernel_short->set_order_w(order_w);
        bmnet_permute_forward_bmkernel_short->set_need_permute_(need_permute_);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_flatten_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    int input_dim_len,
    int* input_dim)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_flatten_forward_bmkernel");
        auto *bmnet_flatten_forward_bmkernel_short = inst->mutable_bmnet_flatten_forward_bmkernel_short();
        bmnet_flatten_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_flatten_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_flatten_forward_bmkernel_short->set_input_dim_len(input_dim_len);
        for (size_t i = 0; i < (size_t)input_dim_len; i++)
            bmnet_flatten_forward_bmkernel_short->add_input_dim(input_dim[i]);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_normalize_forward_bmkernel(
    gaddr_t input_gaddr,
    gaddr_t output_gaddr,
    bool has_scale_blob,
    gaddr_t scale_gaddr,
    bool across_spatial,
    bool channel_shared,
    float eps,
    float* scales,
    int input_n,
    int input_c,
    int input_h,
    int input_w)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_normalize_forward_bmkernel");
        auto *bmnet_normalize_forward_bmkernel_short = inst->mutable_bmnet_normalize_forward_bmkernel_short();
        bmnet_normalize_forward_bmkernel_short->set_input_gaddr(input_gaddr);
        bmnet_normalize_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_normalize_forward_bmkernel_short->set_has_scale_blob(has_scale_blob);
        bmnet_normalize_forward_bmkernel_short->set_scale_gaddr(scale_gaddr);
        bmnet_normalize_forward_bmkernel_short->set_across_spatial(across_spatial);
        bmnet_normalize_forward_bmkernel_short->set_channel_shared(channel_shared);
        bmnet_normalize_forward_bmkernel_short->set_eps(eps);
        for (size_t i = 0; i < (size_t)input_c; i++)
            bmnet_normalize_forward_bmkernel_short->add_scales(scales[i]);
        bmnet_normalize_forward_bmkernel_short->set_input_n(input_n);
        bmnet_normalize_forward_bmkernel_short->set_input_c(input_c);
        bmnet_normalize_forward_bmkernel_short->set_input_h(input_h);
        bmnet_normalize_forward_bmkernel_short->set_input_w(input_w);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_deconv_forward_bmkernel(
    gaddr_t ga_ifmap,
    gaddr_t ga_ofmap,
    gaddr_t ga_weight,
    gaddr_t ga_bias,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int groups,
    int output_c,
    int output_h,
    int output_w,
    int kh,
    int kw,
    int dh,
    int dw,
    int pad_h,
    int pad_w,
    int stride_h,
    int stride_w,
    bool using_bias,
    bool result_add,
    bool do_relu)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_deconv_forward_bmkernel");
        auto *bmnet_deconv_forward_bmkernel_short = inst->mutable_bmnet_deconv_forward_bmkernel_short();
        bmnet_deconv_forward_bmkernel_short->set_ga_ifmap(ga_ifmap);
        bmnet_deconv_forward_bmkernel_short->set_ga_ofmap(ga_ofmap);
        bmnet_deconv_forward_bmkernel_short->set_ga_weight(ga_weight);
        bmnet_deconv_forward_bmkernel_short->set_ga_bias(ga_bias);
        bmnet_deconv_forward_bmkernel_short->set_input_n(input_n);
        bmnet_deconv_forward_bmkernel_short->set_input_c(input_c);
        bmnet_deconv_forward_bmkernel_short->set_input_h(input_h);
        bmnet_deconv_forward_bmkernel_short->set_input_w(input_w);
        bmnet_deconv_forward_bmkernel_short->set_groups(groups);
        bmnet_deconv_forward_bmkernel_short->set_output_c(output_c);
        bmnet_deconv_forward_bmkernel_short->set_output_h(output_h);
        bmnet_deconv_forward_bmkernel_short->set_output_w(output_w);
        bmnet_deconv_forward_bmkernel_short->set_kh(kh);
        bmnet_deconv_forward_bmkernel_short->set_kw(kw);
        bmnet_deconv_forward_bmkernel_short->set_dh(dh);
        bmnet_deconv_forward_bmkernel_short->set_dw(dw);
        bmnet_deconv_forward_bmkernel_short->set_pad_h(pad_h);
        bmnet_deconv_forward_bmkernel_short->set_pad_w(pad_w);
        bmnet_deconv_forward_bmkernel_short->set_stride_h(stride_h);
        bmnet_deconv_forward_bmkernel_short->set_stride_w(stride_w);
        bmnet_deconv_forward_bmkernel_short->set_using_bias(using_bias);
        bmnet_deconv_forward_bmkernel_short->set_result_add(result_add);
        bmnet_deconv_forward_bmkernel_short->set_do_relu(do_relu);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
inline void bmnet_stack_forward_bmkernel(
    gaddr_t* input_gaddrs,
    gaddr_t output_gaddr,
    int input_size,
    int input_n,
    int input_c,
    int input_h,
    int input_w,
    int axis)
{
    // gen asm
    if (asm_context::get_context().on())
    {
        bmnet::bm1680::CommandBuffer buf;
        auto *inst = buf.add_inst();
        auto &name = asm_context::get_context().name;
        if (not name.empty())
            inst->set_name(name);
        name.clear();
        inst->set_type("bmnet_stack_forward_bmkernel");
        auto *bmnet_stack_forward_bmkernel_short = inst->mutable_bmnet_stack_forward_bmkernel_short();
        for (size_t i = 0; i < (size_t)input_size; i++)
            bmnet_stack_forward_bmkernel_short->add_input_gaddrs(input_gaddrs[i]);
        bmnet_stack_forward_bmkernel_short->set_output_gaddr(output_gaddr);
        bmnet_stack_forward_bmkernel_short->set_input_size(input_size);
        bmnet_stack_forward_bmkernel_short->set_input_n(input_n);
        bmnet_stack_forward_bmkernel_short->set_input_c(input_c);
        bmnet_stack_forward_bmkernel_short->set_input_h(input_h);
        bmnet_stack_forward_bmkernel_short->set_input_w(input_w);
        bmnet_stack_forward_bmkernel_short->set_axis(axis);
        asm_context::get_context().get_fp() << buf.DebugString() << std::endl;
    }
}
// clang-format on

} // namespace bmnet_asm
} // namespace bmnet
#endif /* BM168X_BMKERNEL_API_H */