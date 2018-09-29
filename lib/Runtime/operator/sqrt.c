#include <onnc/Runtime/operator/sqrt.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_sqrt_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  
) {
  int32_t size = 1;
	for(int32_t i = 0 ; i < input_X_ndim ; ++i){
		size *= input_X_dims[i];
	}
	for(int32_t i = 0 ; i < size ; ++i){
		output_Y[i] = sqrtf(input_X[i]);
	}
}
