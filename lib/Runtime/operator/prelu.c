#include <onnc/Runtime/operator/prelu.h>

#include <stdint.h>
#include <stdbool.h>

typedef int32_t ONNC_INDEX_TYPE;

#include "generic/assign.h"
#include "generic/binary.h"

static float prelu_(float slope, float x)
{
  return (x >= 0.0f) ? x : x * slope;
}

void ONNC_RUNTIME_prelu_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_slope
  ,int32_t input_slope_ndim, const int32_t * restrict input_slope_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  
) {
        ONNC_ASSIGN(float,
                    output_Y, output_Y_dims, output_Y_ndim,
                    input_slope, input_slope_dims, input_slope_ndim);
        ONNC_BINARY(float,
                    output_Y, output_Y_dims, output_Y_ndim,
                    input_X, input_X_dims, input_X_ndim, prelu_);
}
