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

void ONNC_RUNTIME_split_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * const * restrict output_outputs
  ,int32_t output_outputs_ntensor
  ,const int32_t * output_outputs_ndim, const int32_t * const * restrict output_outputs_dims
  ,int32_t axis
  ,int32_t * restrict split
  ,int32_t number_of_split
) {
  /* initialize parameter following the spec */
  int32_t output_split_number = output_outputs_ntensor;
  int32_t output_split[output_split_number];
  if(number_of_split){
    for(int32_t index = 0 ; index < output_split_number ; index++){
        output_split[index] = split[index];
    }
  }
  else{
    int32_t avg_split = input_input_dims[axis] / output_split_number;
    for(int32_t index = 0 ; index < output_split_number ; index++){
      output_split[index] = avg_split;
    }
  }

  /* acculate index */
  int32_t base = 0;
  for(int32_t index = 0 ; index < output_split_number ; index++){
    output_split[index] = base + output_split[index];
    base = output_split[index];
  }

  /* calculate axisDistance */
  int32_t axisDistance[input_input_ndim];
  int32_t axis_base = axisDistance[input_input_ndim-1] = 1;
  for(int32_t dim = input_input_ndim - 2  ; dim >= 0 ; dim--){
    axisDistance[dim] = axis_base * input_input_dims[dim+1];
    axis_base *= input_input_dims[dim];
  }

  int32_t split_base = 0;
  for(int32_t splitIndex = 0 ; splitIndex < output_split_number ; splitIndex++){
    int32_t output_col = 0;
    forLoop(input_input, 0,
            input_input_ndim, input_input_dims,
            0, axisDistance,
            axis, split_base, output_split[splitIndex],
            output_outputs, splitIndex, &output_col);
    split_base = output_split[splitIndex];
  }
}
