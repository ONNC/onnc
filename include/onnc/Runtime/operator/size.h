#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_size_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_size
  ,int32_t output_size_ndim, const int32_t * restrict output_size_dims
  
);
