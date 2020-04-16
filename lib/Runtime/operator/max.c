#include <onnc/Runtime/operator/max.h>

#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef int32_t ONNC_INDEX_TYPE;

#include "generic/assign.h"
#include "generic/binary.h"

void ONNC_RUNTIME_max_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_max
  ,int32_t output_max_ndim, const int32_t * restrict output_max_dims
  
) {
	// Resize all inputs
	for(int32_t i = 0 ; i < input_data_0_ntensor ; ++i) {
		ONNC_ASSIGN(float, output_max, output_max_dims, output_max_ndim, input_data_0[i], input_data_0_dims[i], input_data_0_ndim[i]);
	}

	int32_t tensor_len = 1;
	for(int32_t i = 0 ; i < output_max_ndim ; ++i) {
		tensor_len *= output_max_dims[i];
	}

	// Initialize output with minimum
	for(int32_t i = 0 ; i < tensor_len ; ++i) {
		output_max[i] = FLT_MIN;
	}

	// Choose the maximum
	for(int32_t i = 0 ; i < input_data_0_ntensor ; ++i) {
		ONNC_BINARY(float, output_max, output_max_dims, output_max_ndim, input_data_0[i], input_data_0_dims[i], input_data_0_ndim[i], fmax);
	}
}
