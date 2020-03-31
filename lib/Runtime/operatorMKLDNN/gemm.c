#include <onnc/Runtime/operatorMKLDNN/gemm.h>

#include <stdint.h>

mkldnn_engine_t engine;
mkldnn_stream_t stream;

void ONNC_RUNTIME_gemm_float(void* restrict context,
    const float* restrict A, int32_t Adim, const int32_t* restrict Ashape,
    const float* restrict B, int32_t Bdim, const int32_t* restrict Bshape,
    const float* restrict C, int32_t Cdim, const int32_t* restrict Cshape,
    float* restrict Y, int32_t Ydim, const int32_t* restrict Yshape,
    float alpha, float beta, int32_t transA, int32_t transB)
{
  mkldnn_engine_create(&engine, mkldnn_cpu, 0);
  mkldnn_stream_create(&stream, engine, mkldnn_stream_default_flags); 

  mkldnn_dim_t M = Yshape[0];
  mkldnn_dim_t N = Yshape[1];
  mkldnn_dim_t K = Yshape[0]+Ashape[1]-M;
  if(Cdim == 2)
  {
    memcpy(Y,C,M*N*sizeof(float));
  }
  else
  {
    for(int i=0;i<M;i++)
    {
      memcpy(Y+N*i,C,N*sizeof(float));
    }
  }
  mkldnn_sgemm( (transA?'T':'N'),(transB?'T':'N'), M, N, K, alpha, A, (transA?M:K), B, (transB?K:M), beta, Y, N);
  mkldnn_stream_destroy(stream);
  mkldnn_engine_destroy(engine);
}
