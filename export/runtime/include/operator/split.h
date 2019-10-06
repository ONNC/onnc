#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_split_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * const * restrict output_outputs
  ,int32_t output_outputs_ntensor
  ,const int32_t * output_outputs_ndim, const int32_t * const * restrict output_outputs_dims
  ,int32_t axis
  ,int32_t * restrict split
  ,int32_t number_of_split
);
