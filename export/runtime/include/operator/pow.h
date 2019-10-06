#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_pow_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_Y
  ,int32_t input_Y_ndim, const int32_t * restrict input_Y_dims
  ,float * restrict output_Z
  ,int32_t output_Z_ndim, const int32_t * restrict output_Z_dims
  
);
