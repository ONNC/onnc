#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_loop_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_M
  ,int32_t input_M_ndim, const int32_t * restrict input_M_dims
  ,const float * restrict input_cond
  ,int32_t input_cond_ndim, const int32_t * restrict input_cond_dims
  ,const float * const * restrict input_v_initial
  ,int32_t input_v_initial_ntensor
  ,const int32_t * input_v_initial_ndim, const int32_t * const * restrict input_v_initial_dims
  ,float * const * restrict output_v_final_and_scan_outputs
  ,int32_t output_v_final_and_scan_outputs_ntensor
  ,const int32_t * output_v_final_and_scan_outputs_ndim, const int32_t * const * restrict output_v_final_and_scan_outputs_dims
  ,const void * restrict body
);
