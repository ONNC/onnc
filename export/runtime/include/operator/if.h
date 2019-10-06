#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_if_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_cond
  ,int32_t input_cond_ndim, const int32_t * restrict input_cond_dims
  ,float * const * restrict output_outputs
  ,int32_t output_outputs_ntensor
  ,const int32_t * output_outputs_ndim, const int32_t * const * restrict output_outputs_dims
  ,const void * restrict else_branch
  ,const void * restrict then_branch
);
