#include <onnc/Runtime/operator/upsample.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static size_t getOffset(const int32_t* restrict target_dims, int32_t ndim, const int32_t* restrict dims)
{
  size_t offset = target_dims[ndim - 1] % dims[ndim - 1];
  size_t steps  = 1;
  for (int32_t i = ndim - 2; i >= 0; --i) {
    steps *= dims[i + 1];
    offset += target_dims[i] * steps;
  }
  return offset;
}

static void nearest_upsample(int32_t output_Y_ndim, const int32_t* restrict output_Y_dims,
                             const int32_t* restrict output_convs, const int32_t* restrict input_convs,
                             float* restrict scales, const float* restrict input_X, float* restrict output_Y)
{
  int32_t output_size = output_convs[0] * output_Y_dims[0];
  for (int32_t i = 0; i < output_size; ++i) {
    // Convert output offset to input coordination
    int32_t input_coord[output_Y_ndim];
    for (int32_t j = 0, output_offset = i; j < output_Y_ndim; ++j) {
      input_coord[j] = output_offset / output_convs[j] / scales[j];
      output_offset %= output_convs[j];
    }
    // Convert input coordination to input offset
    int32_t input_offset = 0;
    for (int32_t j = 0; j < output_Y_ndim; ++j) {
      input_offset += input_coord[j] * input_convs[j];
    }
    output_Y[i] = input_X[input_offset];
  }
}

#include "internal/upsample.inc"