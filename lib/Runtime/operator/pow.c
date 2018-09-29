#include <onnc/Runtime/operator/pow.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_pow_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_Y
  ,int32_t input_Y_ndim, const int32_t * restrict input_Y_dims
  ,float * restrict output_Z
  ,int32_t output_Z_ndim, const int32_t * restrict output_Z_dims
  
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < input_X_ndim ; dim++){
      size *= input_X_dims[dim];
  }

  for(int32_t index = 0 ; index < size ; index++){
      output_Z[index] = pow(input_X[index], input_Y[index]);
  }
}
