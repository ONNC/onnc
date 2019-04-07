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

#include "internal/slice.inc"