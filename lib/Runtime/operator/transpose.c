#include <onnc/Runtime/operator/transpose.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_transpose_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_transposed
  ,int32_t output_transposed_ndim, const int32_t * restrict output_transposed_dims
  ,int32_t * restrict perm
  ,int32_t number_of_perm
) {
  //Variable Declarations
  int32_t elem = 1, ndim = input_data_ndim;
  for (int32_t i = 0; i < input_data_ndim; i++){
    elem *= input_data_dims[i];
  }
  //Coordinate for original position
  //New Coordinate with Transpose
  int32_t coordinate[elem][ndim];
  int32_t newCoor[elem][ndim];
  for(int32_t i = 0; i < elem; i++){
    for(int32_t j = 0; j < ndim; j++){
      coordinate[i][j] = 0;
      newCoor[i][j] = 0;
    }
  }
  // Old Dimension
  //Size that a dimension can hold
  int32_t dimSize[ndim];
  for(int32_t i = 0; i < ndim; i++){
    dimSize[i] = input_data_dims[i];
  }
  //Stride allocate 1d array to multidimensinal array
  int32_t dimStride[ndim];
  dimStride[ndim - 1] = 1;
  for(int32_t i = ndim - 2; i >= 0; i--){
      dimStride[i] = dimStride[i+1] * dimSize[i+1];
  }
  //Use algorithm to store coordinate to the coordinate array
  int32_t dimCounter = 0;
  int32_t indexCounter = 0;
  for(int32_t i = 0; i < elem; i++){
    indexCounter = i;
    dimCounter = 0;
    while(dimCounter < ndim){
      coordinate[i][dimCounter] = indexCounter / dimStride[dimCounter];
      indexCounter %= dimStride[dimCounter];
      dimCounter++;
    }
  }
  //New Dimension
  //New data dimensions by permutation
  int32_t newDims[ndim];
  for(int32_t i = 0; i < ndim; i++){
    newDims[i] = input_data_dims[perm[i]];
  }
  //New data size
  int32_t newDimSize[ndim];
  for(int32_t i = 0; i < ndim; i++){
    newDimSize[i] = newDims[i];
  }
  //New Stride
  int32_t newDimStride[ndim];
  newDimStride[ndim-1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    newDimStride[i] = newDimStride[i+1] * newDimSize[i+1];
  }
  //Get the new Coordinate formed by new dimension adter transpose
  dimCounter = 0;
  indexCounter = 0;
  for(int32_t i = 0; i < elem; i++){
    indexCounter = i;
    dimCounter = 0;
    while(dimCounter < ndim){
      newCoor[i][dimCounter] = indexCounter / newDimStride[dimCounter];
      indexCounter %= newDimStride[dimCounter];
      dimCounter++;
    }
  }

  //To transpose a matrix, swap the axes by permutation
  int32_t finalCoor[elem][ndim];
  for(int32_t i = 0; i < elem; i++){
    for(int32_t j = 0; j < ndim; j++){
      finalCoor[i][j] = newCoor[i][perm[j]];
    }
  }

  //To find match value between final coordinate and original coordinate array
  int32_t counter = 0;
  for(int32_t k = 0; k < elem; k++){
    for(int32_t i = 0; i < elem; i++){
      counter = 0;
      for(int32_t j = 0; j < ndim; j++){
        if(coordinate[i][j] == finalCoor[k][j]){
          counter++;
        }       
      }
      if(counter == ndim){
        output_transposed[k] = input_data[i];
      }
    }
  }
}
