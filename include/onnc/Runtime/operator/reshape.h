#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_reshape_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,const float * restrict input_shape
  ,int32_t input_shape_ndim, const int32_t * restrict input_shape_dims
  ,float * restrict output_reshaped
  ,int32_t output_reshaped_ndim, const int32_t * restrict output_reshaped_dims
  
);
