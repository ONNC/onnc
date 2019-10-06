#include <onnc/Runtime/operator/reducemin.h>

#include <stdint.h>
#include <stdbool.h>
#include <float.h>

void ONNC_RUNTIME_reducemin_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
) {
  int32_t ndim = input_data_ndim;
  int32_t nk_ndim = input_data_ndim - number_of_axes;

  //output elements
  int32_t nk_output_elem = 1;
  int32_t input_elem = 1;


  int32_t nk_output_dims[nk_ndim];
  int32_t ip = 0;
  int32_t counter;
  for(int32_t i = 0; i < ndim; i++){
    counter = 0;
    for(int32_t j = 0; j < number_of_axes; j++){
      if(i != axes[j]) counter++;
    }
    if(counter == number_of_axes){
      nk_output_dims[ip] = input_data_dims[i];
      nk_output_elem *= nk_output_dims[ip];
      ip++;
    }
    input_elem *= input_data_dims[i];
  }

  //Result array
  float result[nk_output_elem];

  //input stride, for reference
  int32_t InDimStride[ndim];
  InDimStride[ndim-1] = 1;
  for(int32_t i = (ndim - 1) - 1; i >= 0; i--){
    InDimStride[i] = InDimStride[i+1] * input_data_dims[i+1];
  }

  int32_t OutDimStride[nk_ndim];
  OutDimStride[nk_ndim-1] = 1;
  for(int32_t i = (nk_ndim - 1) - 1; i >= 0; i--){
    OutDimStride[i] = OutDimStride[i+1] * nk_output_dims[i+1];
  }

  //reduce process keepdim = false
  //To locate coordinates, count the strides!

  //iteration setup
  int32_t inDimCounter = 0;
  int32_t outDimCounter= 0;
  int32_t inIndexCounter = 0;
  int32_t outIndexCounter = 0;

  int32_t index = 0;
  float min_temp[nk_output_elem];

  for(int32_t i = 0; i < nk_output_elem; i++){
    min_temp[i] = FLT_MAX;
  }
  //This 3 loop using top down algorithm to reduce the max 
  //locate index , transform to constiguous array index
  //ommit the AXES DIMENTION in order to obtain MAX in certain vector
  for(int32_t i = 0; i < input_elem; i++){
    inIndexCounter = i;
    outIndexCounter = 0;
    inDimCounter = 0;
    outDimCounter = 0;
    while(inDimCounter < ndim){
      counter = 0;
      index = inIndexCounter / InDimStride[inDimCounter];
      inIndexCounter %= InDimStride[inDimCounter];
      for(int32_t j = 0; j < number_of_axes; j++){
        if(inDimCounter != axes[j]) counter++;
      }
      if(counter == number_of_axes){
          outIndexCounter += (index * OutDimStride[outDimCounter]);
          outDimCounter++;
      }
      inDimCounter++;
    }
    if(input_data[i] <= min_temp[outIndexCounter]){
      //SOLUTION
      min_temp[outIndexCounter] = input_data[i];
    }
    result[outIndexCounter] = min_temp[outIndexCounter];
  }
  //keep dimension
  if(keepdims == 1){
    for(int32_t i = 0; i < input_elem; i++){
      inIndexCounter = i;
      outIndexCounter = 0;
      inDimCounter = 0;
      outDimCounter = 0;
      while(inDimCounter < ndim){
        counter = 0;
        index = inIndexCounter / InDimStride[inDimCounter];
        inIndexCounter %= InDimStride[inDimCounter];
        for(int32_t j = 0; j < number_of_axes; j++){
          if(inDimCounter != axes[j]) counter++;
        }
        if(counter == number_of_axes){
          outIndexCounter += (index * OutDimStride[outDimCounter]);
          outDimCounter++;
        }
        inDimCounter++;
      }
      output_reduced[i] = result[outIndexCounter];
    }
  }else if(keepdims == 0){
    for(int32_t i = 0; i < nk_output_elem; i++){
      output_reduced[i] = result[i];
    }
  }
}
