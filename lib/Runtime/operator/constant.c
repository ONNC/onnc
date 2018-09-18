#include <onnc/Runtime/operator/constant.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_constant_float(
  void * restrict onnc_runtime_context
  
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,const void * restrict value
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < output_output_ndim ; dim++){
    size *= output_output_dims[dim];
  }

  float fill = *((float*)value);
  for(int32_t index = 0 ; index < size ; index++){
    output_output[index] = fill;
  }
}
