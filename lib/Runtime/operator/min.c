#include <onnc/Runtime/operator/min.h>

#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef int32_t ONNC_INDEX_TYPE;

#include "generic/assign.h"
#include "generic/binary.h"

void ONNC_RUNTIME_min_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_min
  ,int32_t output_min_ndim, const int32_t * restrict output_min_dims
  
) {
	// Resize all inputs
	for(int32_t i = 0 ; i < input_data_0_ntensor ; ++i) {
		ONNC_ASSIGN(float, output_min, output_min_dims, output_min_ndim, input_data_0[i], input_data_0_dims[i], input_data_0_ndim[i]);
	}

	int32_t tensor_len = 1;
	for(int32_t i = 0 ; i < output_min_ndim ; ++i) {
		tensor_len *= output_min_dims[i];
	}

	// Initialize output with maximum
	for(int32_t i = 0 ; i < tensor_len ; ++i) {
		output_min[i] = FLT_MAX;
	}

	// Choose the minimum
	for(int32_t i = 0 ; i < input_data_0_ntensor ; ++i) {
		ONNC_BINARY(float, output_min, output_min_dims, output_min_ndim, input_data_0[i], input_data_0_dims[i], input_data_0_ndim[i], fmin);
	}
}
