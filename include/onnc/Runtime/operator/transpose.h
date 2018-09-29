#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_transpose_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_transposed
  ,int32_t output_transposed_ndim, const int32_t * restrict output_transposed_dims
  ,int32_t * restrict perm
  ,int32_t number_of_perm
);
