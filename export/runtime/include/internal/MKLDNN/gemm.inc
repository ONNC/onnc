void ONNC_RUNTIME_gemm_float(void* restrict onnc_runtime_context, const float* restrict input_A, int32_t input_A_ndim,
        const int32_t* restrict input_A_dims, const float* restrict input_B, int32_t input_B_ndim,
        const int32_t* restrict input_B_dims, const float* restrict input_C, int32_t input_C_ndim,
        const int32_t* restrict input_C_dims, float* restrict output_Y, int32_t output_Y_ndim,
        const int32_t* restrict output_Y_dims, float alpha, float beta, int32_t transA,
        int32_t transB)
{
  mkldnn_dim_t M = output_Y_dims[0];
  mkldnn_dim_t N = output_Y_dims[1];
  mkldnn_dim_t K = input_A_dims[0]+input_A_dims[1]-M;
  if(input_C_ndim == 2)
    memcpy(output_Y,input_C,M*N*sizeof(float));
  else
    for(int i=0;i<M;i++)
      memcpy(output_Y+N*i,input_C,N*sizeof(float));
  mkldnn_sgemm( (transA?'T':'N'),(transB?'T':'N'), M, N, K, alpha, input_A, (transA?M:K), input_B, (transB?K:M), beta, output_Y, N);
}
