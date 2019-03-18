#include <onnc/Runtime/operator/slice.h>

#include <stdint.h>
#include <stdbool.h>

static void forLoop(
    int32_t axisIndex, int32_t ndim
    , const int32_t * restrict lowerBound, const int32_t *  restrict uperBound
    , const float * input, int32_t inputIndex
    , float * output, int32_t* outputIndex
    ,int32_t * axisDistance
){
  if(axisIndex == ndim){
      int32_t outIndex = *outputIndex;
      output[outIndex] = input[inputIndex];
      *outputIndex = outIndex + 1;
      return;
  }
  for(int32_t dimIndex = lowerBound[axisIndex] ; dimIndex < uperBound[axisIndex]; dimIndex++){
    forLoop(axisIndex + 1, ndim, lowerBound, uperBound,
            input, inputIndex + dimIndex * axisDistance[axisIndex],
            output, outputIndex, axisDistance);
  }
}

void ONNC_RUNTIME_slice_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t * restrict ends
  ,int32_t number_of_ends
  ,int32_t * restrict starts
  ,int32_t number_of_starts
) {
  int32_t output_dim_lower[input_data_ndim];
  int32_t output_dim_higher[input_data_ndim];
  int32_t output_axes[input_data_ndim];

  /* initialize */
  for(int32_t index = 0 ; index < input_data_ndim ; index++){
    output_dim_lower[index] = -1;
    output_dim_higher[index] = -1;
  }

  /* process axes info */
  if(number_of_axes == 0){
    number_of_axes = input_data_ndim;
    for(int32_t axis = 0 ; axis < number_of_axes ; axis++){
      output_axes[axis] = axis;
    }
  }else{
    for(int32_t axis = 0 ; axis < number_of_axes ; axis++){
      output_axes[axis] = axes[axis];
    }
  }

  /* process start and endIndex */
  for(int32_t axis = 0 ; axis < number_of_axes ; axis++){
    int32_t dimIndex = output_axes[axis];

    /* for lower */
    int32_t n = input_data_dims[axis];
    if(starts[axis] > n) output_dim_lower[dimIndex] = n;
    else if(starts[axis] < 0) output_dim_lower[dimIndex] = starts[axis] + n;
    else output_dim_lower[dimIndex] = starts[axis];

    /* for higher */
    if(ends[axis] > n) output_dim_higher[dimIndex] = n;
    else if(ends[axis] < 0) output_dim_higher[dimIndex] = ends[axis] + n;
    else output_dim_higher[dimIndex] = ends[axis];
  }

  /* fill the empty axis*/
  for(int32_t index = 0 ; index < input_data_ndim ; index++){
    if(output_dim_lower[index] == -1) output_dim_lower[index] = 0;
    if(output_dim_higher[index] == -1) output_dim_higher[index] = input_data_dims[index];
  }

  /* compute axisDistance */
  int32_t axisDistance[input_data_ndim];
  int32_t base = axisDistance[input_data_ndim-1] = 1;
  for(int32_t dim = input_data_ndim - 2  ; dim >= 0 ; dim--){
    axisDistance[dim] = base * input_data_dims[dim+1];
    base *= input_data_dims[dim];
  }

  /* for loop */
  int32_t outputIndex = 0;
  forLoop(0, input_data_ndim, output_dim_lower, output_dim_higher,
          input_data, 0, output_output, &outputIndex, axisDistance);
}
