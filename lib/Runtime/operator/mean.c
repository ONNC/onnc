#include <onnc/Runtime/operator/mean.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_mean_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_mean
  ,int32_t output_mean_ndim, const int32_t * restrict output_mean_dims
  
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < input_data_0_ndim[0]; dim++){
    size *= input_data_0_dims[0][dim];
  }

  for(int32_t index = 0 ; index < size ; index ++){
    output_mean[index] = 0;
    for(int32_t tensorIndex = 0 ; tensorIndex < input_data_0_ntensor ; tensorIndex++){
        output_mean[index] += input_data_0[tensorIndex][index];
    }
    output_mean[index] /= input_data_0_ntensor;
  }
}
