#pragma once

#include <stdint.h>
#include <stdbool.h>

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
