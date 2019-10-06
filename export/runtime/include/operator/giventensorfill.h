#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_giventensorfill_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_shape
  ,int32_t input_shape_ndim, const int32_t * restrict input_shape_dims
  ,float * restrict output_X
  ,int32_t output_X_ndim, const int32_t * restrict output_X_dims
  ,int32_t * restrict extra_shape
  ,int32_t number_of_extra_shape
  ,int32_t input_as_shape
  ,int32_t * restrict shape
  ,int32_t number_of_shape
  ,float * restrict values
  ,int32_t number_of_values
);
