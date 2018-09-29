#include <onnc/Runtime/operator/lpnormalization.h>

#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

void ONNC_RUNTIME_lpnormalization_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t axis
  ,int32_t p
) {
  //calculate new dimension
  int32_t ndim = input_input_ndim;

  //ndim corresponding to axis
  //axis accept neg value
  if(axis < 0){
    axis += ndim;
  } 

  //output elements
  int32_t output_elem = 1;
  int32_t input_elem = 1;
  int32_t output_dims[ndim - 1];

  int32_t ip = 0;
  for(int32_t i = 0; i < ndim; i++){
    if(i != axis){
      output_elem *= input_input_dims[i];
      output_dims[ip] = input_input_dims[i];
      ip++;
    }
    input_elem *= input_input_dims[i];
  }
  
  //input stride, for reference
  int32_t InDimStride[ndim];
  InDimStride[ndim-1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    InDimStride[i] = InDimStride[i+1] * input_input_dims[i+1];
  }

  int32_t out_ndim = ndim -1;
  int32_t OutDimStride[out_ndim];
  OutDimStride[out_ndim-1] = 1;
  for(int32_t i = (out_ndim - 1) - 1; i >= 0; i--){
    OutDimStride[i] = OutDimStride[i+1] * output_dims[i+1];
  }

  //p-norm process
  //To locate coordinates, count the strides!
  int32_t inDimCounter = 0;
  int32_t outDimCounter= 0;

  int32_t inIndexCounter = 0;
  int32_t outIndexCounter = 0;

  float summation[output_elem];
  for(int32_t i = 0; i < output_elem; i++){
    summation[i] = 0.0f;
  }

  //Forumala 
  int32_t index = 0;
  for(int32_t j = 0; j < input_elem; j++){
    inIndexCounter = j;
    outIndexCounter = 0;
    outDimCounter = 0;
    inDimCounter = 0;
    while(inDimCounter < ndim){
      index = inIndexCounter / InDimStride[inDimCounter];
      inIndexCounter %= InDimStride[inDimCounter];
      if(inDimCounter != axis){
        outIndexCounter += (index * OutDimStride[outDimCounter]);
        outDimCounter++;
      }
      inDimCounter++;
    }
    summation[outIndexCounter] += fabs(powf(input_input[j], p));
  }

  //Formula
  for(int32_t i = 0; i < output_elem; i++){
    output_output[i] = powf(summation[i], (1.0f/(float)p)); 
  }
}
