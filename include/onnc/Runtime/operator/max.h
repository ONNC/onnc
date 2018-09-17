#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_max_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_max
  ,int32_t output_max_ndim, const int32_t * restrict output_max_dims
  
);
