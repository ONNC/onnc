#pragma once

#include <stdint.h>
#include <stdbool.h>

void ONNC_RUNTIME_maxroipool_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,const float * restrict input_rois
  ,int32_t input_rois_ndim, const int32_t * restrict input_rois_dims
  ,float * restrict output_Y
  ,int32_t output_Y_ndim, const int32_t * restrict output_Y_dims
  ,int32_t * restrict pooled_shape
  ,int32_t number_of_pooled_shape
  ,float spatial_scale
);
