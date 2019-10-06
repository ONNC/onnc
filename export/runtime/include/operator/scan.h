#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_scan_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_sequence_lens
  ,int32_t input_sequence_lens_ndim, const int32_t * restrict input_sequence_lens_dims
  ,const float * const * restrict input_initial_state_and_scan_inputs
  ,int32_t input_initial_state_and_scan_inputs_ntensor
  ,const int32_t * input_initial_state_and_scan_inputs_ndim, const int32_t * const * restrict input_initial_state_and_scan_inputs_dims
  ,float * const * restrict output_final_state_and_scan_outputs
  ,int32_t output_final_state_and_scan_outputs_ntensor
  ,const int32_t * output_final_state_and_scan_outputs_ndim, const int32_t * const * restrict output_final_state_and_scan_outputs_dims
  ,const void * restrict body
  ,int32_t * restrict directions
  ,int32_t number_of_directions
  ,int32_t num_scan_inputs
);
