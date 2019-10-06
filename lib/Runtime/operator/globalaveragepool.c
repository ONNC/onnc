#include <onnc/Runtime/operator/globalaveragepool.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_globalaveragepool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  
) {
  int32_t nbatch = output_Y_dims[0];
  int32_t nchannel = output_Y_dims[1];

  int32_t elem = 1;
  for(int32_t i = 0; i < input_X_ndim; i++){
    elem *= input_X_dims[i];
  }

  int32_t dimStride[input_X_ndim];
  dimStride[input_X_ndim-1] = 1;
  for(int32_t i = (input_X_ndim - 1) - 1; i >= 0; i--){
    dimStride[i] = dimStride[i+1] * input_X_dims[i+1];
  }

  int32_t avgSize = elem / (nbatch * nchannel);

  float sum[nbatch][nchannel];
  for(int32_t i = 0; i < nbatch; i++){
    for(int32_t j = 0; j < nchannel; j++){
      sum[i][j] = 0;
    }
  }

  int32_t indexCounter;
  int32_t dimCounter;
  int32_t index[2];
  for(int32_t i = 0; i < elem; i++){
    indexCounter = i;
    dimCounter = 0;
    while(dimCounter < 2){
      index[dimCounter] = indexCounter / dimStride[dimCounter];
      indexCounter %= dimStride[dimCounter];
      dimCounter++;
    }
    sum[index[0]][index[1]] += input_X[i];
  }

  for(int32_t i = 0; i < nbatch; i++){
    for(int32_t j = 0; j < nchannel; j++){
      output_Y[i * nchannel + j] = sum[i][j] / (float)avgSize;
    }
  }
}
