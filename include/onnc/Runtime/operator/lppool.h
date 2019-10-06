#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_lppool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict auto_pad
  ,int32_t * restrict kernel_shape
  ,int32_t number_of_kernel_shape
  ,int32_t p
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,int32_t * restrict strides
  ,int32_t number_of_strides
);
