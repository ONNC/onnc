void ONNC_RUNTIME_reducesum_float(void* restrict onnc_runtime_context, const float* restrict input_data,
                                  int32_t input_data_ndim, const int32_t* restrict input_data_dims,
                                  float* restrict output_reduced, int32_t output_reduced_ndim,
                                  const int32_t* restrict output_reduced_dims, int32_t* restrict axes,
                                  int32_t number_of_axes, int32_t keepdims)
{
  int32_t in_size = 1;
  for (int32_t i = 0; i < input_data_ndim; ++i) {
    in_size *= input_data_dims[i];
  }
  float   A[in_size], B[in_size];
  int32_t A_ndim, B_ndim;
  A_ndim = B_ndim = input_data_ndim;
  int32_t A_dims[A_ndim], B_dims[B_ndim];
  int32_t meofarr[A_ndim];
  for (int32_t i = 0; i < in_size; ++i)
    A[i] = input_data[i];
  for (int32_t i = 0; i < A_ndim; ++i)
    A_dims[i] = B_dims[i] = input_data_dims[i];
  for (int32_t i = 0; i < number_of_axes - 1; i += 2) {
    Reduce(A, B, A_ndim, A_dims, axes[i], meofarr, 0);
    A_dims[axes[i]] = B_dims[axes[i]] = 1;
    Reduce(B, A, B_ndim, B_dims, axes[i + 1], meofarr, 0);
    A_dims[axes[i + 1]] = B_dims[axes[i + 1]] = 1;
  }
  int32_t ou_size = 1;
  for (int32_t i = 0; i < output_reduced_ndim; ++i) {
    ou_size *= output_reduced_dims[i];
  }
  if (number_of_axes & 1) {
    Reduce(A, B, A_ndim, A_dims, axes[number_of_axes - 1], meofarr, 0);
    for (int32_t i = 0; i < ou_size; ++i) {
      output_reduced[i] = B[i];
    }
  } else {
    for (int32_t i = 0; i < ou_size; ++i) {
      output_reduced[i] = A[i];
    }
  }
}
