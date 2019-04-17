void ONNC_RUNTIME_averagepool_float(void* restrict onnc_runtime_context, const float* restrict input_X,
                                    int32_t input_X_ndim, const int32_t* restrict input_X_dims,
                                    float* restrict output_Y, int32_t output_Y_ndim,
                                    const int32_t* restrict output_Y_dims, const char* restrict auto_pad,
                                    int32_t count_include_pad, int32_t* restrict kernel_shape,
                                    int32_t number_of_kernel_shape, int32_t* restrict pads, int32_t number_of_pads,
                                    int32_t* restrict strides, int32_t number_of_strides)
{
  // TODO auto_pad
  int64_t size = 1;
  for (int i = 0; i < input_X_ndim - 2; ++i) {
    size *= kernel_shape[i];
  }

  int32_t o_dim[input_X_ndim];
  memset(o_dim, 0, sizeof(o_dim));
  do { // while o_dim
    int32_t base_dim[input_X_ndim];
    for (int32_t i = 2; i < input_X_ndim; ++i) {
      base_dim[i] = o_dim[i] * strides[i - 2] - pads[i - 2];
    }

    float sum = 0.f;

    int32_t k_dim[input_X_ndim - 2];
    memset(k_dim, 0, sizeof(k_dim));
    int32_t padCount = 0;
    do { // while k_dim
      int32_t i_dim[input_X_ndim];
      i_dim[0] = o_dim[0]; // N
      i_dim[1] = o_dim[1]; // C
      for (int32_t i = 2; i < input_X_ndim; ++i) {
        i_dim[i] = base_dim[i] + k_dim[i - 2];
      }
      int32_t isPad = 0;
      sum += get_value_or_zero_5(input_X_ndim, input_X_dims, input_X, i_dim, &isPad);
      if (isPad) {
        ++padCount;
      }
    } while (next_dim_no_check(input_X_ndim - 2, k_dim, kernel_shape));
    if (count_include_pad) {
      sum /= size;
    } else {
      sum /= (size - padCount);
    }

    output_Y[dim_to_offset_with_step(input_X_ndim, o_dim, output_Y_dims)] = sum;
  } while (next_dim_no_check(input_X_ndim, o_dim, output_Y_dims));
}
