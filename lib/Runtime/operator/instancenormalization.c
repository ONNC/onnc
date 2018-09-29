#include <onnc/Runtime/operator/instancenormalization.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_instancenormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,const float * restrict input_scale
  ,int32_t input_scale_ndim, const int32_t * restrict input_scale_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float epsilon
) {
  int32_t nbatch = input_input_dims[0];
  int32_t nchannel = input_input_dims[1];
  int32_t ndim = input_input_ndim;
  
  //The tensor is base on the number of batch and the number of channel
  float xtile_sum[nbatch][nchannel];
  float xtile_square_sum[nbatch][nchannel];
  float mean[nbatch][nchannel];
  float variance[nbatch][nchannel];
 
  //H x W for 4 dims
  //D1 x D2 .. for more than 4 dims
  //Suppose mean and variance is calculated per batch and channel ,initialize them
  for(int32_t i =0; i < nbatch; i++){
    for(int32_t j =0; j < nchannel; j++){
      xtile_sum[i][j] = .0f;
      xtile_square_sum[i][j] = .0f;
    }
  }

  //Get dataSize : elem
  int32_t elem = 1;
  for(int32_t i =0; i < ndim; i++){
    elem *= input_input_dims[i];
  }

  //Get stride for locate coordinate purpose
  int32_t dimStride[ndim];
  dimStride[ndim - 1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    dimStride[i] = dimStride[i+1] * input_input_dims[i+1];
  }

  //Group the element-wise data to first two dimension
  //To calculate summation of xtile and xtile^2 (for mean and variance) 
  int32_t coor[elem][2];
  int32_t indexCounter;
  int32_t dimCounter;
  for(int32_t i = 0; i < elem; i++){
    indexCounter = i;
    dimCounter = 0;
    //0 , 1 dims are batch and channel
    //Sum other multidimensional value
    while(dimCounter < 2){
      //element-wise coordinate record here
      coor[i][dimCounter] = indexCounter / dimStride[dimCounter];
      indexCounter %= dimStride[dimCounter];
      dimCounter++;
    }
    //summation x
    xtile_sum[coor[i][0]][coor[i][1]] += input_input[i];
    //summation x^2
    xtile_square_sum[coor[i][0]][coor[i][1]] += powf(input_input[i], 2);
  }

  // Obtain mean and variance per channel and batch
  for(int32_t i = 0; i < nbatch; i++){
    for(int32_t j = 0; j < nchannel; j++){
      mean[i][j] = 1.0f / (float)(nbatch * nchannel) * xtile_sum[i][j];
      variance[i][j] = 1.0f / (float)(nbatch * nchannel) * xtile_square_sum[i][j] - powf(mean[i][j],2);
    }
  }

  //Formula
  //y = scale * (x - mean) / sqrt(variance + epsilon) + B
  for(int32_t i = 0; i < elem; i++){
    //Note that BIAS and SCALE are only per channel
    //coor[i][1] is Channel index
    output_output[i] = input_scale[coor[i][1]] * (input_input[i] - mean[coor[i][1]][coor[i][1]]) / sqrtf(variance[coor[i][1]][coor[i][1]] - epsilon) + input_B[coor[i][1]];
  }
}
