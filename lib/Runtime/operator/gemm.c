#include <onnc/Runtime/operator/gemm.h>

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_gemm_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_A
  ,int32_t input_A_ndim, const int32_t * restrict input_A_dims
  ,const float * restrict input_B
  ,int32_t input_B_ndim, const int32_t * restrict input_B_dims
  ,const float * restrict input_C
  ,int32_t input_C_ndim, const int32_t * restrict input_C_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
  ,int32_t transA
  ,int32_t transB
) {
  int num = transA ? input_A_dims[0] : input_A_dims[1];
  for (int32_t i = 0 ; i < output_Y_dims[0] ; ++i) {
    for (int32_t j = 0 ; j < output_Y_dims[1] ; ++j) {
      output_Y[ i * output_Y_dims[1] + j ] = 0;
      for (int32_t k = 0 ; k < num ; ++k) {
        output_Y[ i * output_Y_dims[1] + j ] +=
          input_A[ transA ? (k * input_A_dims[1] + i) :
                            (i * input_A_dims[1] + k) ] *
          input_B[ transB ? (j * input_B_dims[1] + k) :
                            (k * input_B_dims[1] + j) ];
      }
      output_Y[ i * output_Y_dims[1] + j ] *= alpha;
      output_Y[ i * output_Y_dims[1] + j ] += beta * input_C[ + j ];
    }
  }
}
