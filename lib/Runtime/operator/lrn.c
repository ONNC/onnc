#include <onnc/Runtime/operator/lrn.h>

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void ONNC_RUNTIME_lrn_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,float alpha
  ,float beta
  ,float bias
  ,int32_t size
) {
  // XXX: WFT ONNX.
  // (bias+(alpha/size)*sum(xi^2 for every xi in the local region))^beta
  float alpha_over_size = alpha / size;

  int64_t N = input_X_dims[0];
  int64_t C = input_X_dims[1];
  int64_t len = 1;
  for (int32_t i = 2; i < input_X_ndim; ++i) {
    len *= input_X_dims[i];
  }
  for (int64_t n = 0; n < N; ++n) {
    for (int64_t c = 0; c < C; ++c) {
      for (int64_t i = 0; i < len; ++i) {
        int64_t start = c - (size/2);
        if (start < 0) { start = 0; }
        int64_t end = c + (size/2);
        if (end >= C) { end = C - 1; }

        float sum = 0.f;
        for (int64_t j = start; j <= end; ++j) {
          float value = input_X[(n*C + j)*len + i];
          sum += value * value;
        }
        output_Y[(n*C + c)*len + i] = input_X[(n*C + c)*len + i] * powf(bias + alpha_over_size * sum, -beta);
      }
    }
  }
}
