#include <onnc/Runtime/operator/sum.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_sum_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_sum
  ,int32_t output_sum_ndim, const int32_t * restrict output_sum_dims
  
) {
  int64_t size = 1;
  for (int32_t i = 0; i < output_sum_ndim; ++i) {
    size *= output_sum_dims[i];
  }
  for (int64_t i = 0; i < size; ++i) {
    output_sum[i] = 0.f;
  }
  // TODO: Broadcasting
  for (int64_t i = 0; i < input_data_0_ntensor; ++i) {
    for (int64_t j = 0; j < size; ++j) {
      output_sum[j] += input_data_0[i][j];
    }
  }
}
