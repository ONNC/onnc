void ONNC_RUNTIME_gemm_float(void* restrict onnc_runtime_context, const float* restrict input_A, int32_t input_A_ndim,
        const int32_t* restrict input_A_dims, const float* restrict input_B, int32_t input_B_ndim,
        const int32_t* restrict input_B_dims, const float* restrict input_C, int32_t input_C_ndim,
        const int32_t* restrict input_C_dims, float* restrict output_Y, int32_t output_Y_ndim,
        const int32_t* restrict output_Y_dims, float alpha, float beta, int32_t transA,
        int32_t transB)
{
#ifdef MKLDNN

    mkldnn_dim_t M = output_Y_dims[0];
    mkldnn_dim_t N = output_Y_dims[1];
    mkldnn_dim_t K = input_A_dims[0]+input_A_dims[1]-M;
    if(input_C_ndim == 2)
        memcpy(output_Y,input_C,M*N*sizeof(float));
    else
        for(int i=0;i<M;i++)
            memcpy(output_Y+N*i,input_C,N*sizeof(float));
    mkldnn_sgemm( (transA)?'T':'N',(transB)?'T':'N', M, N, K, alpha, input_A, (transA)?M:K, input_B, (transB)?K:M, beta, output_Y, N);
#else
    int num = transA ? input_A_dims[0] : input_A_dims[1];
    for (int32_t i = 0; i < output_Y_dims[0]; ++i) {
        for (int32_t j = 0; j < output_Y_dims[1]; ++j) {
            output_Y[i * output_Y_dims[1] + j] = 0;
            for (int32_t k = 0; k < num; ++k) {
                output_Y[i * output_Y_dims[1] + j] += input_A[transA ? (k * input_A_dims[1] + i) : (i * input_A_dims[1] + k)] *
                    input_B[transB ? (j * input_B_dims[1] + k) : (k * input_B_dims[1] + j)];
            }
            output_Y[i * output_Y_dims[1] + j] *= alpha;
            output_Y[i * output_Y_dims[1] + j] += beta * input_C[+j];
        }
    }
#endif
}
