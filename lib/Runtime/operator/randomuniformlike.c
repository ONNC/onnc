#include <onnc/Runtime/operator/randomuniformlike.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void ONNC_RUNTIME_randomuniformlike_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t dtype
  ,float high
  ,float low
  ,float seed
) {
  srand(seed ? seed : time(NULL));
  int32_t size = 1;
  for(int32_t dim = 0; dim < output_output_ndim; dim++){
    size *= output_output_dims[dim];
  }
  for(int32_t in = 0 ; in < size ; ++in ){
    output_output[in] = ((float)rand() / (float)RAND_MAX) * (high - low) + low;
  }
}
