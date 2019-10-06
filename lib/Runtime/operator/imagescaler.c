#include <onnc/Runtime/operator/imagescaler.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_imagescaler_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,float * restrict bias
  ,int32_t number_of_bias
  ,float scale
) {
  int32_t xN = input_input_dims[0];
  int32_t xC = input_input_dims[1];
  int32_t elemSize = 1;

  for(int32_t i = 2; i < input_input_ndim; ++i){
    elemSize *= input_input_dims[i];
  }

  for(int32_t iN = 0; iN < xN; ++iN){
    for(int32_t iC = 0; iC < xC; ++iC){
      const float *pX = input_input   + iN * xC * elemSize + iC * elemSize;
            float *pY = output_output + iN * xC * elemSize + iC * elemSize;
      float cur_bias = bias[iC];
      for(int32_t i = 0; i < elemSize; ++i){
        pY[i] = scale * pX[i] + cur_bias;
      }
    }
  }
}
