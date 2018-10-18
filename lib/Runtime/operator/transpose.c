#include <onnc/Runtime/operator/transpose.h>

#include <stdint.h>
#include <stdio.h>

void ONNC_RUNTIME_transpose_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_transposed
  ,int32_t output_transposed_ndim, const int32_t * restrict output_transposed_dims
  ,int32_t * restrict perm
  ,int32_t number_of_perm
) {
  // Prepare
  int32_t elemCount = 1;
  int32_t coordSize[input_data_ndim];
  for (int32_t i = 0; i < input_data_ndim; i++){
    coordSize[input_data_ndim - 1 - i] = elemCount;
    elemCount *= input_data_dims[i];
  }
  // TODO: Check if perm duplicate or not;
  for (int32_t rawIndex = 0; rawIndex < elemCount; ++rawIndex){
    int32_t coord[input_data_ndim];
    // Coordinate
    for(int32_t i = 0, tmpRaw = rawIndex; i < input_data_ndim; ++i){
      coord[i] = tmpRaw / coordSize[i];
      tmpRaw %= coordSize[i];
    }
    // Assign to destination
    int32_t destIndex = 0;
    for(int32_t i = 0; i < input_data_ndim; ++i){
      destIndex += coord[perm[i]] * coordSize[i];
    }
    output_transposed[rawIndex] = input_data[destIndex];
  }
}
