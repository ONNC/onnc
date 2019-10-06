#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_shape_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_shape
  ,int32_t output_shape_ndim, const int32_t * restrict output_shape_dims
  
);
