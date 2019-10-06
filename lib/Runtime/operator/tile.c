#include <onnc/Runtime/operator/tile.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_tile_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,const float * restrict input_repeats
  ,int32_t input_repeats_ndim, const int32_t * restrict input_repeats_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  
) {
  //calculate new dimension
  int32_t ndim = input_input_ndim;
  int32_t newDims[ndim];
  for (int32_t i = 0; i < ndim; i++){
    //By definition input_repeats should be an integer
    newDims[i] = input_input_dims[i] * (int32_t)input_repeats[i];
  }

  //output elements
  int32_t elem = 1;
  for(int32_t i = 0; i < ndim; i++){
    elem *= newDims[i];
  }

  //input stride, for reference
  int32_t dimStride[ndim];
  dimStride[ndim-1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    dimStride[i] = dimStride[i+1] * input_input_dims[i+1];
  }

  //output stride, for calculating new coordinates
  int32_t newDimStride[ndim];
  newDimStride[ndim-1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    newDimStride[i] = newDimStride[i+1] * newDims[i+1];
  }

  //To locate coordinates, count the strides!
  int32_t coor[elem][ndim];
  int32_t dimCounter = 0;
  int32_t indexCounter =0;
  for(int32_t i = 0; i < elem; i++){
      indexCounter = i;
      dimCounter = 0;
      while(dimCounter < ndim){
          coor[i][dimCounter] = indexCounter / newDimStride[dimCounter];
          indexCounter %= newDimStride[dimCounter];
          dimCounter++;
      }
  }

  //every new coordinate correspond to original input offset
  int32_t correspondIndex = 0;
  for(int32_t i = 0; i < elem; i++){
    correspondIndex = 0;
    for(int32_t j = 0; j < ndim; j++){
      //mapping operation
      correspondIndex += ((coor[i][j] % input_input_dims[j]) * dimStride[j]);
    }
    //output
    output_output[i] = input_input[correspondIndex];
  }
}
