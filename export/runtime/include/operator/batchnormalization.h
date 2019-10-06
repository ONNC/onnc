#pragma once

#include <stdint.h>
#include <stdbool.h>

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
