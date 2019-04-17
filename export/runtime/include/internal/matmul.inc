void Enu(const float* restrict A, const int32_t* restrict A_dims, const float* restrict B,
                  const int32_t* restrict B_dims, float* restrict C, int32_t C_ndim, const int32_t* restrict C_dims,
                  int32_t* meofarr, int32_t idx, int32_t num)
{
  if (idx == C_ndim) {
    int32_t ou_idx = con(meofarr, C_ndim, C_dims);
    C[ou_idx]      = 0;
    int32_t _1     = meofarr[C_ndim - 1];
    int32_t _2     = meofarr[C_ndim - 2];
    for (int32_t i = 0; i < num; ++i) {
      meofarr[C_ndim - 1] = i;
      int32_t A_idx       = con(meofarr, C_ndim, A_dims);
      meofarr[C_ndim - 1] = _1;
      meofarr[C_ndim - 2] = i;
      int32_t B_idx       = con(meofarr, C_ndim, B_dims);
      C[ou_idx] += A[A_idx] * B[B_idx];
      meofarr[C_ndim - 1] = _1;
      meofarr[C_ndim - 2] = _2;
    }
  } else {
    for (int32_t i = 0; i < C_dims[idx]; ++i) {
      meofarr[idx] = i;
      Enu(A, A_dims, B, B_dims, C, C_ndim, C_dims, meofarr, idx + 1, num);
    }
  }
}

#include "matmul.impl.inc"
