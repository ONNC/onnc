#pragma once

#include <stdint.h>
#include <stdbool.h>

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
