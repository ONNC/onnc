void ONNC_RUNTIME_sum_float(void* restrict onnc_runtime_context, const float* const* restrict input_data_0,
                            int32_t input_data_0_ntensor, const int32_t* input_data_0_ndim,
                            const int32_t* const* restrict input_data_0_dims, float* restrict output_sum,
                            int32_t output_sum_ndim, const int32_t* restrict output_sum_dims

)
{
  const int64_t outputSize = getTensorSize(output_sum_ndim, output_sum_dims);
  for (int64_t i = 0; i < outputSize; ++i) {
    output_sum[i] = 0.f;
  }
  // TODO: Broadcasting
  for (int64_t i = 0; i < input_data_0_ntensor; ++i) {
    const int64_t inputSize = getTensorSize(input_data_0_ndim[i], input_data_0_dims[i]);
    const int64_t minSize   = outputSize < inputSize ? outputSize : inputSize;
    for (int64_t j = 0; j < minSize; ++j) {
      output_sum[j] += input_data_0[i][j];
    }
  }
}
