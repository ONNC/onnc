#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_topk_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Values
  ,int32_t output_Values_ndim, const int32_t * restrict output_Values_dims
  ,float * restrict output_Indices
  ,int32_t output_Indices_ndim, const int32_t * restrict output_Indices_dims
  ,int32_t axis
  ,int32_t k
);
