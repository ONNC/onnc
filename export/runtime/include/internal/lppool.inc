void ONNC_RUNTIME_lppool_float(void* restrict onnc_runtime_context, const float* restrict input_X, int32_t input_X_ndim,
                               const int32_t* restrict input_X_dims, float* restrict output_Y, int32_t output_Y_ndim,
                               const int32_t* restrict output_Y_dims, const char* restrict auto_pad,
                               int32_t* restrict kernel_shape, int32_t number_of_kernel_shape, int32_t p,
                               int32_t* restrict pads, int32_t number_of_pads, int32_t* restrict strides,
                               int32_t number_of_strides)
{
  int32_t ou_meofarr[output_Y_ndim];
  for (int32_t i = 2; i < output_Y_ndim; ++i) {
    ou_meofarr[i] = 0;
  }
  int32_t in_meofarr[input_X_ndim];
  for (int32_t i = 0; i < output_Y_dims[0]; ++i) {
    for (int32_t j = 0; j < output_Y_dims[1]; ++j) {
      ou_meofarr[0] = i;
      ou_meofarr[1] = j;
      Enu_Out(input_X, input_X_ndim, input_X_dims, in_meofarr, output_Y, output_Y_ndim, output_Y_dims, ou_meofarr, 2,
              kernel_shape, p, pads, strides);
    }
  }
}
