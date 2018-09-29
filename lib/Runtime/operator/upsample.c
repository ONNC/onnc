#include <onnc/Runtime/operator/upsample.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static size_t getOffset(
  const int32_t * restrict target_dims,
  int32_t ndim,
  const int32_t * restrict dims
){
  size_t offset = target_dims[ndim - 1] % dims[ndim - 1];
  size_t steps = 1;
  for(int32_t i = ndim - 2; i >= 0; --i){
    steps *= dims[i + 1];
    offset += target_dims[i] * steps;
  }
  return offset;
}

static void nearest_upsample(
  int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const int32_t * restrict output_convs
  ,const int32_t * restrict input_convs
  ,float * restrict scales
  ,const float * restrict input_X
  ,float * restrict output_Y
){
  int32_t output_size = output_convs[0] * output_Y_dims[0];
  for(int32_t i = 0; i < output_size; ++i){
    // Convert output offset to input coordination
    int32_t input_coord[output_Y_ndim];
    for(int32_t j = 0, output_offset = i; j < output_Y_ndim; ++j){
      input_coord[j] = output_offset / output_convs[j] / scales[j];
      output_offset %= output_convs[j];
    }
    // Convert input coordination to input offset
    int32_t input_offset = 0;
    for(int32_t j = 0; j < output_Y_ndim; ++j){
      input_offset += input_coord[j] * input_convs[j];
    }
    output_Y[i] = input_X[input_offset];
  }
}

void ONNC_RUNTIME_upsample_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,const char * restrict mode
  ,float * restrict scales
  ,int32_t number_of_scales
) {
  // Calculate output conversion
  int32_t output_convs[output_Y_ndim];
  output_convs[output_Y_ndim - 1] = 1;
  for(int32_t i = output_Y_ndim - 2; i >= 0; --i){
    output_convs[i] = output_convs[i + 1] * output_Y_dims[i + 1];
  }
  // Calculate input conversion
  int32_t input_convs[input_X_ndim];
  input_convs[input_X_ndim - 1] = 1;
  for(int32_t i = input_X_ndim - 2; i >= 0; --i){
    input_convs[i] = input_convs[i + 1] * input_X_dims[i + 1];
  }
  // Nearest
  if(mode == NULL || !strcmp(mode, "nearest")){
    nearest_upsample(
      output_Y_ndim
      ,output_Y_dims
      ,output_convs
      ,input_convs
      ,scales
      ,input_X
      ,output_Y
    );
  }else{
    // TODO: linear
  }
}
