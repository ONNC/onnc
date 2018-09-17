#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_gruunit_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_hidden_prev
  ,int32_t input_hidden_prev_ndim, const int32_t * restrict input_hidden_prev_dims
  ,const float * restrict input_gates
  ,int32_t input_gates_ndim, const int32_t * restrict input_gates_dims
  ,const float * restrict input_seq_lengths
  ,int32_t input_seq_lengths_ndim, const int32_t * restrict input_seq_lengths_dims
  ,const float * restrict input_t
  ,int32_t input_t_ndim, const int32_t * restrict input_t_dims
  ,float * restrict output_hidden
  ,int32_t output_hidden_ndim, const int32_t * restrict output_hidden_dims
  ,int32_t drop_states
);
