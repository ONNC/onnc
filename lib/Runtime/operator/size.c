#include <onnc/Runtime/operator/size.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_size_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_size
  ,int32_t output_size_ndim, const int32_t * restrict output_size_dims
  
) {
  int32_t size = 1;
  for(int32_t dim = 0 ; dim < input_data_ndim ; dim++){
    size *= input_data_dims[dim];
  }
  *output_size = size;
}
