#include <onnc/Runtime/operator/exp.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_exp_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < input_input_ndim ; dim++){
    size *= input_input_dims[dim];
  }

  for(int32_t index = 0 ; index < size ; index++){
    output_output[index] = expf(input_input[index]);
  }
}
