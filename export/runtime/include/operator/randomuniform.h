#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_randomuniform_float(
  void * restrict onnc_runtime_context
  
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float high
  ,float low
  ,float seed
  ,int32_t * restrict shape
  ,int32_t number_of_shape
);
