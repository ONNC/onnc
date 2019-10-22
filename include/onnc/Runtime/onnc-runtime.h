#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct ONNC_RUNTIME_tensor_file
{
  void* data; /* Implementation defined data */
};

struct ONNC_RUNTIME_tensor_offset {
  uint64_t offset; /* Tensor offset */
  uint64_t size;   /* Size of tensor in bytes */
};

#ifndef ONNC_RUNTIME_TENSOR_FILE_MAGIC
#  define ONNC_RUNTIME_TENSOR_FILE_MAGIC ".TSR"
#endif

struct ONNC_RUNTIME_tensor_offset_table
{
  char                              magic[8]; /* Tensor File magic number. */
  uint64_t                          number_of_tensors;
  struct ONNC_RUNTIME_tensor_offset tensor_offsets[];
};

struct ONNC_RUNTIME_tensor_view
{
  void*    data;
  uint64_t size; /* Size of tensor in bytes */
};

// Core Library
bool ONNC_RUNTIME_has_tensor(const struct ONNC_RUNTIME_tensor_offset_table* table, uint64_t tensor);
struct ONNC_RUNTIME_tensor_offset ONNC_RUNTIME_get_tensor_offset(const struct ONNC_RUNTIME_tensor_offset_table* table,
                                                                 uint64_t                                       tensor);

// Client Library
const struct ONNC_RUNTIME_tensor_offset_table*
                                ONNC_RUNTIME_read_tensor_offset_table(struct ONNC_RUNTIME_tensor_file* file);
struct ONNC_RUNTIME_tensor_view ONNC_RUNTIME_read_tensor(struct ONNC_RUNTIME_tensor_file* file, uint64_t tensor);

// Service Library
struct ONNC_RUNTIME_inference_context
{
  struct ONNC_RUNTIME_tensor_file* input;
  struct ONNC_RUNTIME_tensor_file* weight;
  uint64_t                         id;
  void (*completed)(uint64_t id, struct ONNC_RUNTIME_tensor_view output);
};

/**
 * ONNC generated entry point.
 * @param context The ONNC Runtime Context.
 */
int model_main(const struct ONNC_RUNTIME_inference_context* context);

/**
 * Initialize runtime.
 * @deprecated
 * @return The ONNC Runtime Context, should be passed to every ONNC Runtime functions.
 */
void *ONNC_RUNTIME_init_runtime();

/**
 * Shutdown runtime.
 * @deprecated
 * @param onnc_runtime_context The ONNC Runtime Context.
 * @return True if shutdown successfully. False if something wrong.
 */
bool ONNC_RUNTIME_shutdown_runtime(void *onnc_runtime_context);

void ONNC_RUNTIME_abs_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_acos_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_add_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_affine_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
);
void ONNC_RUNTIME_and_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_argmax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t axis
  ,int32_t keepdims
);
void ONNC_RUNTIME_argmin_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t axis
  ,int32_t keepdims
);
void ONNC_RUNTIME_asin_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_atan_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_aten_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_input
  ,int32_t input_input_ntensor
  ,const int32_t * input_input_ndim, const int32_t * const * restrict input_input_dims
  ,float * const * restrict output_output
  ,int32_t output_output_ntensor
  ,const int32_t * output_output_ndim, const int32_t * const * restrict output_output_dims
);
void ONNC_RUNTIME_averagepool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t count_include_pad
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
void ONNC_RUNTIME_batchnormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_scale
  ,int32_t input_scale_ndim, const int32_t * restrict input_scale_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_mean
  ,int32_t input_mean_ndim, const int32_t * restrict input_mean_dims
  ,const float * restrict input_var
  ,int32_t input_var_ndim, const int32_t * restrict input_var_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float * restrict output_mean
  ,int32_t output_mean_ndim, const int32_t * restrict output_mean_dims
  ,float * restrict output_var
  ,int32_t output_var_ndim, const int32_t * restrict output_var_dims
  ,float * restrict output_saved_mean
  ,int32_t output_saved_mean_ndim, const int32_t * restrict output_saved_mean_dims
  ,float * restrict output_saved_var
  ,int32_t output_saved_var_ndim, const int32_t * restrict output_saved_var_dims
  ,float epsilon
  ,float momentum
  ,int32_t spatial
);
void ONNC_RUNTIME_cast_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t to
);
void ONNC_RUNTIME_ceil_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_clip_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float max
  ,float min
);
void ONNC_RUNTIME_concat_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_inputs
  ,int32_t input_inputs_ntensor
  ,const int32_t * input_inputs_ndim, const int32_t * const * restrict input_inputs_dims
  ,float * restrict output_concat_result
  ,int32_t output_concat_result_ndim, const int32_t * restrict output_concat_result_dims
  ,int32_t axis
);
void ONNC_RUNTIME_constantfill_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,int32_t * restrict extra_shape
  ,int32_t number_of_extra_shape
  ,int32_t input_as_shape
  ,int32_t * restrict shape
  ,int32_t number_of_shape
  ,float value
);
void ONNC_RUNTIME_constant_float(
  void * restrict onnc_runtime_context
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,const void * restrict value
);
void ONNC_RUNTIME_conv_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict dilations
  ,int32_t number_of_dilations
  ,int32_t group
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
void ONNC_RUNTIME_convtranspose_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict dilations
  ,int32_t number_of_dilations
  ,int32_t group
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict output_padding
  ,int32_t number_of_output_padding
  ,int32_t * restrict output_shape
  ,int32_t number_of_output_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
void ONNC_RUNTIME_cos_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_crop_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t * restrict border
  ,int32_t number_of_border
  ,int32_t * restrict scale
  ,int32_t number_of_scale
);
void ONNC_RUNTIME_depthtospace_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t blocksize
);
void ONNC_RUNTIME_div_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_dropout_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float * restrict output_mask
  ,int32_t output_mask_ndim, const int32_t * restrict output_mask_dims
  ,float ratio
);
void ONNC_RUNTIME_elu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
);
void ONNC_RUNTIME_equal_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_expand_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,const float * restrict input_shape
  ,int32_t input_shape_ndim, const int32_t * restrict input_shape_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_exp_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_flatten_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
);
void ONNC_RUNTIME_floor_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_gather_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,const float * restrict input_indices
  ,int32_t input_indices_ndim, const int32_t * restrict input_indices_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
);
void ONNC_RUNTIME_gemm_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_C
  ,int32_t input_C_ndim, const int32_t * restrict input_C_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
  ,int32_t transA
  ,int32_t transB
);
void ONNC_RUNTIME_giventensorfill_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_shape
  ,int32_t input_shape_ndim, const int32_t * restrict input_shape_dims
  ,float * restrict output_X
  ,int32_t output_X_ndim, const int32_t * restrict output_X_dims
  ,int32_t * restrict extra_shape
  ,int32_t number_of_extra_shape
  ,int32_t input_as_shape
  ,int32_t * restrict shape
  ,int32_t number_of_shape
  ,float * restrict values
  ,int32_t number_of_values
);
void ONNC_RUNTIME_globalaveragepool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_globallppool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,int32_t p
);
void ONNC_RUNTIME_globalmaxpool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_greater_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_gru_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_R
  ,int32_t input_R_ndim, const int32_t * restrict input_R_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_sequence_lens
  ,int32_t input_sequence_lens_ndim, const int32_t * restrict input_sequence_lens_dims
  ,const float * restrict input_initial_h
  ,int32_t input_initial_h_ndim, const int32_t * restrict input_initial_h_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float * restrict output_Y_h
  ,int32_t output_Y_h_ndim, const int32_t * restrict output_Y_h_dims
  ,float * restrict activation_alpha
  ,int32_t number_of_activation_alpha
  ,float * restrict activation_beta
  ,int32_t number_of_activation_beta
  ,const char ** restrict activations
  ,int32_t number_of_activations
  ,float clip
  ,const char * restrict direction
  ,int32_t hidden_size
  ,int32_t linear_before_reset
);
void ONNC_RUNTIME_gruunit_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_hidden_prev
  ,int32_t input_hidden_prev_ndim, const int32_t * restrict input_hidden_prev_dims
  ,const float * restrict input_gates
  ,int32_t input_gates_ndim, const int32_t * restrict input_gates_dims
  ,const float * restrict input_seq_lengths
  ,int32_t input_seq_lengths_ndim, const int32_t * restrict input_seq_lengths_dims
  ,const float * restrict input_t
  ,int32_t input_t_ndim, const int32_t * restrict input_t_dims
  ,float * restrict output_hidden
  ,int32_t output_hidden_ndim, const int32_t * restrict output_hidden_dims
  ,int32_t drop_states
);
void ONNC_RUNTIME_hardmax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
);
void ONNC_RUNTIME_hardsigmoid_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
);
void ONNC_RUNTIME_identity_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_if_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_cond
  ,int32_t input_cond_ndim, const int32_t * restrict input_cond_dims
  ,float * const * restrict output_outputs
  ,int32_t output_outputs_ntensor
  ,const int32_t * output_outputs_ndim, const int32_t * const * restrict output_outputs_dims
  ,const void * restrict else_branch
  ,const void * restrict then_branch
);
void ONNC_RUNTIME_imagescaler_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float * restrict bias
  ,int32_t number_of_bias
  ,float scale
);
void ONNC_RUNTIME_instancenormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,const float * restrict input_scale
  ,int32_t input_scale_ndim, const int32_t * restrict input_scale_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float epsilon
);
void ONNC_RUNTIME_leakyrelu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
);
void ONNC_RUNTIME_less_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_log_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_logsoftmax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
);
void ONNC_RUNTIME_loop_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_M
  ,int32_t input_M_ndim, const int32_t * restrict input_M_dims
  ,const float * restrict input_cond
  ,int32_t input_cond_ndim, const int32_t * restrict input_cond_dims
  ,const float * const * restrict input_v_initial
  ,int32_t input_v_initial_ntensor
  ,const int32_t * input_v_initial_ndim, const int32_t * const * restrict input_v_initial_dims
  ,float * const * restrict output_v_final_and_scan_outputs
  ,int32_t output_v_final_and_scan_outputs_ntensor
  ,const int32_t * output_v_final_and_scan_outputs_ndim, const int32_t * const * restrict output_v_final_and_scan_outputs_dims
  ,const void * restrict body
);
void ONNC_RUNTIME_loopindextensor_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_T
  ,int32_t input_T_ndim, const int32_t * restrict input_T_dims
  ,const float * restrict input_loop_idx
  ,int32_t input_loop_idx_ndim, const int32_t * restrict input_loop_idx_dims
  ,float * restrict output_O
  ,int32_t output_O_ndim, const int32_t * restrict output_O_dims
  ,int32_t axis
);
void ONNC_RUNTIME_lpnormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
  ,int32_t p
);
void ONNC_RUNTIME_lppool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t p
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
void ONNC_RUNTIME_lrn_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
  ,float bias
  ,int32_t size
);
void ONNC_RUNTIME_lstm_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_R
  ,int32_t input_R_ndim, const int32_t * restrict input_R_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_sequence_lens
  ,int32_t input_sequence_lens_ndim, const int32_t * restrict input_sequence_lens_dims
  ,const float * restrict input_initial_h
  ,int32_t input_initial_h_ndim, const int32_t * restrict input_initial_h_dims
  ,const float * restrict input_initial_c
  ,int32_t input_initial_c_ndim, const int32_t * restrict input_initial_c_dims
  ,const float * restrict input_P
  ,int32_t input_P_ndim, const int32_t * restrict input_P_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float * restrict output_Y_h
  ,int32_t output_Y_h_ndim, const int32_t * restrict output_Y_h_dims
  ,float * restrict output_Y_c
  ,int32_t output_Y_c_ndim, const int32_t * restrict output_Y_c_dims
  ,float * restrict activation_alpha
  ,int32_t number_of_activation_alpha
  ,float * restrict activation_beta
  ,int32_t number_of_activation_beta
  ,const char ** restrict activations
  ,int32_t number_of_activations
  ,float clip
  ,const char * restrict direction
  ,int32_t hidden_size
  ,int32_t input_forget
);
void ONNC_RUNTIME_matmul_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_max_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_max
  ,int32_t output_max_ndim, const int32_t * restrict output_max_dims
);
void ONNC_RUNTIME_maxpool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float * restrict output_Indices
  ,int32_t output_Indices_ndim, const int32_t * restrict output_Indices_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t storage_order
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
void ONNC_RUNTIME_maxroipool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_rois
  ,int32_t input_rois_ndim, const int32_t * restrict input_rois_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,int32_t * restrict pooled_shape
  ,int32_t number_of_pooled_shape
  ,float spatial_scale
);
void ONNC_RUNTIME_mean_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_mean
  ,int32_t output_mean_ndim, const int32_t * restrict output_mean_dims
);
void ONNC_RUNTIME_meanvariancenormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t across_channels
  ,int32_t normalize_variance
);
void ONNC_RUNTIME_min_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_min
  ,int32_t output_min_ndim, const int32_t * restrict output_min_dims
);
void ONNC_RUNTIME_mul_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_multinomial_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,int32_t sample_size
  ,float seed
);
void ONNC_RUNTIME_neg_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_not_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_or_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_pad_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,const char * restrict mode
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,float value
);
void ONNC_RUNTIME_parametricsoftplus_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
);
void ONNC_RUNTIME_pow_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_Y
  ,int32_t input_Y_ndim, const int32_t * restrict input_Y_dims
  ,float * restrict output_Z
  ,int32_t output_Z_ndim, const int32_t * restrict output_Z_dims
);
void ONNC_RUNTIME_prelu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_slope
  ,int32_t input_slope_ndim, const int32_t * restrict input_slope_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_randomnormal_float(
  void * restrict onnc_runtime_context
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float mean
  ,float scale
  ,float seed
  ,int32_t * restrict shape
  ,int32_t number_of_shape
);
void ONNC_RUNTIME_randomnormallike_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float mean
  ,float scale
  ,float seed
);
void ONNC_RUNTIME_randomuniform_float(
  void * restrict onnc_runtime_context
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float high
  ,float low
  ,float seed
  ,int32_t * restrict shape
  ,int32_t number_of_shape
);
void ONNC_RUNTIME_randomuniformlike_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float high
  ,float low
  ,float seed
);
void ONNC_RUNTIME_reciprocal_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_reducel1_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducel2_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducelogsumexp_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducelogsum_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducemax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducemean_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducemin_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reduceprod_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducesum_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_reducesumsquare_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
);
void ONNC_RUNTIME_relu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_reshape_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,const float * restrict input_shape
  ,int32_t input_shape_ndim, const int32_t * restrict input_shape_dims
  ,float * restrict output_reshaped
  ,int32_t output_reshaped_ndim, const int32_t * restrict output_reshaped_dims
);
void ONNC_RUNTIME_rnn_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_W
  ,int32_t input_W_ndim, const int32_t * restrict input_W_dims
  ,const float * restrict input_R
  ,int32_t input_R_ndim, const int32_t * restrict input_R_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_sequence_lens
  ,int32_t input_sequence_lens_ndim, const int32_t * restrict input_sequence_lens_dims
  ,const float * restrict input_initial_h
  ,int32_t input_initial_h_ndim, const int32_t * restrict input_initial_h_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float * restrict output_Y_h
  ,int32_t output_Y_h_ndim, const int32_t * restrict output_Y_h_dims
  ,float * restrict activation_alpha
  ,int32_t number_of_activation_alpha
  ,float * restrict activation_beta
  ,int32_t number_of_activation_beta
  ,const char ** restrict activations
  ,int32_t number_of_activations
  ,float clip
  ,const char * restrict direction
  ,int32_t hidden_size
);
void ONNC_RUNTIME_scaledtanh_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float alpha
  ,float beta
);
void ONNC_RUNTIME_scale_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float scale
);
void ONNC_RUNTIME_scan_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_sequence_lens
  ,int32_t input_sequence_lens_ndim, const int32_t * restrict input_sequence_lens_dims
  ,const float * const * restrict input_initial_state_and_scan_inputs
  ,int32_t input_initial_state_and_scan_inputs_ntensor
  ,const int32_t * input_initial_state_and_scan_inputs_ndim, const int32_t * const * restrict input_initial_state_and_scan_inputs_dims
  ,float * const * restrict output_final_state_and_scan_outputs
  ,int32_t output_final_state_and_scan_outputs_ntensor
  ,const int32_t * output_final_state_and_scan_outputs_ndim, const int32_t * const * restrict output_final_state_and_scan_outputs_dims
  ,const void * restrict body
  ,int32_t * restrict directions
  ,int32_t number_of_directions
  ,int32_t num_scan_inputs
);
void ONNC_RUNTIME_selu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float gamma
);
void ONNC_RUNTIME_shape_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_shape
  ,int32_t output_shape_ndim, const int32_t * restrict output_shape_dims
);
void ONNC_RUNTIME_sigmoid_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_sin_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_size_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_size
  ,int32_t output_size_ndim, const int32_t * restrict output_size_dims
);
void ONNC_RUNTIME_slice_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t * restrict ends
  ,int32_t number_of_ends
  ,int32_t * restrict starts
  ,int32_t number_of_starts
);
void ONNC_RUNTIME_softmax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
);
void ONNC_RUNTIME_softplus_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_softsign_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_spacetodepth_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t blocksize
);
void ONNC_RUNTIME_split_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * const * restrict output_outputs
  ,int32_t output_outputs_ntensor
  ,const int32_t * output_outputs_ndim, const int32_t * const * restrict output_outputs_dims
  ,int32_t axis
  ,int32_t * restrict split
  ,int32_t number_of_split
);
void ONNC_RUNTIME_sqrt_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
);
void ONNC_RUNTIME_squeeze_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_squeezed
  ,int32_t output_squeezed_ndim, const int32_t * restrict output_squeezed_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
);
void ONNC_RUNTIME_sub_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
void ONNC_RUNTIME_sum_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_sum
  ,int32_t output_sum_ndim, const int32_t * restrict output_sum_dims
);
void ONNC_RUNTIME_tan_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_tanh_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_thresholdedrelu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
);
void ONNC_RUNTIME_tile_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,const float * restrict input_repeats
  ,int32_t input_repeats_ndim, const int32_t * restrict input_repeats_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
);
void ONNC_RUNTIME_topk_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Values
  ,int32_t output_Values_ndim, const int32_t * restrict output_Values_dims
  ,float * restrict output_Indices
  ,int32_t output_Indices_ndim, const int32_t * restrict output_Indices_dims
  ,int32_t axis
  ,int32_t k
);
void ONNC_RUNTIME_transpose_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_transposed
  ,int32_t output_transposed_ndim, const int32_t * restrict output_transposed_dims
  ,int32_t * restrict perm
  ,int32_t number_of_perm
);
void ONNC_RUNTIME_unsqueeze_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_expanded
  ,int32_t output_expanded_ndim, const int32_t * restrict output_expanded_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
);
void ONNC_RUNTIME_upsample_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict mode
  ,float * restrict scales
  ,int32_t number_of_scales
);
void ONNC_RUNTIME_xor_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
);
