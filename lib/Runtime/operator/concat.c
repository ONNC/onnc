#include <onnc/Runtime/operator/concat.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_concat_float(
  void * restrict onnc_runtime_context
  ,const float * const * restrict input_inputs
  ,int32_t input_inputs_ntensor
  ,const int32_t * input_inputs_ndim, const int32_t * const * restrict input_inputs_dims
  ,float * restrict output_concat_result
  ,int32_t output_concat_result_ndim, const int32_t * restrict output_concat_result_dims
  ,int32_t axis
) {
  int32_t elementDistance[input_inputs_ntensor];
  for(int32_t ntensor = 0 ; ntensor < input_inputs_ntensor ; ntensor++){
    elementDistance[ntensor] = 1;
    for(int32_t dim = axis ; dim < input_inputs_ndim[ntensor] ; dim++){
      elementDistance[ntensor] *= input_inputs_dims[ntensor][dim];
    }
  }

  int32_t size[input_inputs_ntensor];
  for(int32_t ntensor = 0 ; ntensor < input_inputs_ntensor ; ntensor++){
    size[ntensor] = elementDistance[ntensor];
    for(int32_t dim = 0 ; dim < axis ; dim++){
      size[ntensor] *= input_inputs_dims[ntensor][dim];
    }
  }

  int32_t axisDim = input_inputs_dims[0][axis];
  int32_t axisIndex = 0;
  int32_t insertIndex = 0;
  while(axisIndex < axisDim){
    for(int32_t ntensor = 0; ntensor < input_inputs_ntensor ; ntensor++){
      int32_t base = axisIndex * elementDistance[ntensor];
      int32_t index = base;
      int32_t nElementDistance = elementDistance[ntensor];
      while(index < size[ntensor]){
        output_concat_result[insertIndex] = input_inputs[ntensor][index];
        insertIndex++;
        if(index % nElementDistance == nElementDistance - 1){
          index = index + 1 - nElementDistance + nElementDistance * axisDim;
        }
        else{
          ++index;
        }
      }
    }
    ++axisIndex;
  }
}
