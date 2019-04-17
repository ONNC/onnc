void ONNC_RUNTIME_maxpool_float(void* restrict onnc_runtime_context, const float* restrict input_X,
                                int32_t input_X_ndim, const int32_t* restrict input_X_dims, float* restrict output_Y,
                                int32_t output_Y_ndim, const int32_t* restrict output_Y_dims,
                                float* restrict output_Indices, int32_t output_Indices_ndim,
                                const int32_t* restrict output_Indices_dims, const char* restrict auto_pad,
                                int32_t* restrict kernel_shape, int32_t number_of_kernel_shape, int32_t* restrict pads,
                                int32_t number_of_pads, int32_t storage_order, int32_t* restrict strides,
                                int32_t number_of_strides)
{
  assert(input_X_ndim == output_Y_ndim);
  int32_t ndim = input_X_ndim;

  int32_t o_dim[ndim];
  memset(o_dim, 0, sizeof(o_dim));
  do { // while o_dim
    int32_t base_dim[ndim];
    for (int32_t i = 2; i < ndim; ++i) {
      base_dim[i] = o_dim[i] * strides[i - 2] - pads[i - 2];
    }

    float max = -FLT_MAX;

    int32_t k_dim[ndim - 2];
    memset(k_dim, 0, sizeof(k_dim));
    do { // while k_dim
      int32_t i_dim[ndim];
      i_dim[0] = o_dim[0]; // N
      i_dim[1] = o_dim[1]; // C
      for (int32_t i = 2; i < ndim; ++i) {
        i_dim[i] = base_dim[i] + k_dim[i - 2];
      }
      float input = get_value_or_zero(ndim, input_X_dims, input_X, i_dim);
      max         = fmaxf(input, max);
    } while (next_dim_no_check(ndim - 2, k_dim, kernel_shape));

    output_Y[dim_to_offset_with_step(ndim, o_dim, output_Y_dims)] = max;
  } while (next_dim_no_check(ndim, o_dim, output_Y_dims));
}
