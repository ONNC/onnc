#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_concat_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_inputs
  ,int32_t input_inputs_ntensor
  ,const int32_t * input_inputs_ndim, const int32_t * const * restrict input_inputs_dims
  ,float * restrict output_concat_result
  ,int32_t output_concat_result_ndim, const int32_t * restrict output_concat_result_dims
  ,int32_t axis
);
