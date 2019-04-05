#include <onnc/Runtime/operator/split.h>

#include <stdint.h>
#include <stdbool.h>

static void forLoop(
    const float * restrict input_input, int32_t input_index,
    int32_t input_input_ndim, const int32_t * restrict input_input_dims,
    int32_t dimIndex, int32_t * restrict axisDistance,
    int32_t axis, int32_t axisLower, int32_t axisHigher,
    float * const * restrict output, int32_t output_row, int32_t * restrict output_col
){
  if(dimIndex == input_input_ndim){
    int32_t col = *output_col;
    output[output_row][col] = input_input[input_index];
    *output_col = col + 1;
    return;
  }
  /* set loop lower and uper when encountering speicific axis */
  int32_t loopLower = 0, loopUper = input_input_dims[dimIndex];
  if(dimIndex == axis){
    loopLower = axisLower;
    loopUper = axisHigher;
  }
  for(int32_t dim = loopLower ; dim < loopUper ; dim++){
      forLoop(input_input, input_index + dim * axisDistance[dimIndex],
              input_input_ndim, input_input_dims,
              dimIndex + 1, axisDistance,
              axis, axisLower, axisHigher,
              output, output_row, output_col);
  }
}

#include "split.inc"