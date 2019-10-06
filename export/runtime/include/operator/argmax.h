#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_argmax_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t axis
  ,int32_t keepdims
);
