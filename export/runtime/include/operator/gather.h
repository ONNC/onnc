#pragma once

#include <stdint.h>
#include <stdbool.h>

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
