#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_loopindextensor_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_T
  ,int32_t input_T_ndim, const int32_t * restrict input_T_dims
  ,const float * restrict input_loop_idx
  ,int32_t input_loop_idx_ndim, const int32_t * restrict input_loop_idx_dims
  ,float * restrict output_O
  ,int32_t output_O_ndim, const int32_t * restrict output_O_dims
  ,int32_t axis
);
