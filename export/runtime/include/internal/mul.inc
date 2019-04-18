void ONNC_RUNTIME_mul_float(void* restrict onnc_runtime_context, const float* restrict input_A, int32_t input_A_ndim,
                            const int32_t* restrict input_A_dims, const float* restrict input_B, int32_t input_B_ndim,
                            const int32_t* restrict input_B_dims, float* restrict output_C, int32_t output_C_ndim,
                            const int32_t* restrict output_C_dims

)
{
  const int64_t sizeOfA = getTensorSize(input_A_ndim, input_A_dims);
  const int64_t sizeOfB = getTensorSize(input_B_ndim, input_B_dims);
  const int64_t sizeOfC = getTensorSize(output_C_ndim, output_C_dims);

  const int64_t minSize = sizeOfA < sizeOfB ? sizeOfA : sizeOfB < sizeOfC ? sizeOfB : sizeOfC;
  for (int64_t i = 0; i < minSize; ++i) {
    output_C[i] = input_A[i] * input_B[i];
  }
  for (int64_t i = minSize; i < sizeOfC; ++i) {
    output_C[i] = 1;
  }
}
