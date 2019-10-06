#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_aten_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_input
  ,int32_t input_input_ntensor
  ,const int32_t * input_input_ndim, const int32_t * const * restrict input_input_dims
  ,float * const * restrict output_output
  ,int32_t output_output_ntensor
  ,const int32_t * output_output_ndim, const int32_t * const * restrict output_output_dims
  
);
