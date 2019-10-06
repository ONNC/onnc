#include <onnc/Runtime/operator/max.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_max_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_data_0
  ,int32_t input_data_0_ntensor
  ,const int32_t * input_data_0_ndim, const int32_t * const * restrict input_data_0_dims
  ,float * restrict output_max
  ,int32_t output_max_ndim, const int32_t * restrict output_max_dims
  
) {
	int32_t ten_len = 1;
	for(int32_t i = 0 ; i < input_data_0_ndim[0] ; ++i){
		ten_len *= input_data_0_dims[0][i];
	}
	for(int32_t j = 0 ; j < ten_len ; ++j){
		output_max[j] = input_data_0[0][j];
		for(int32_t i = 0 ; i < input_data_0_ntensor ; ++i){
			output_max[j] = fmax(output_max[j], input_data_0[i][j]);
		}
	}
}
