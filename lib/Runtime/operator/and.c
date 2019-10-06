#include <onnc/Runtime/operator/and.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_and_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_C
  ,int32_t output_C_ndim, const int32_t * restrict output_C_dims
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < input_A_ndim ; dim++){
      size *= input_A_dims[dim];
  }
  for(int32_t index = 0 ; index < size ; index++){
      output_C[index] = (int32_t)input_A[index] & (int32_t)input_B[index];
  }
}
