void ONNC_RUNTIME_matmul_float(void* restrict onnc_runtime_context, const float* restrict input_A, int32_t input_A_ndim,
                               const int32_t* restrict input_A_dims, const float* restrict input_B,
                               int32_t input_B_ndim, const int32_t* restrict input_B_dims, float* restrict output_Y,
                               int32_t output_Y_ndim, const int32_t* restrict output_Y_dims

)
{
  int32_t meofarr[output_Y_ndim];
  Enu(input_A, input_A_dims, input_B, input_B_dims, output_Y, output_Y_ndim, output_Y_dims, meofarr, 0,
      input_A_dims[input_A_ndim - 1]);
}
