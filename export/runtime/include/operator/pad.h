#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_pad_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,const char * restrict mode
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,float value
);
