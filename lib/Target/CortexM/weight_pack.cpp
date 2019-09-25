#include "weight_pack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

short __gnu_f2h_ieee_vanilla(float param) {
  unsigned int param_bit = *((unsigned int*)(&param));
  // unsigned int param_bit = *(static_cast<unsigned int*>(&param)) ;
  int sign = param_bit >> 31;
  int mantissa = param_bit & 0x007FFFFF;
  int exp = ((param_bit & 0x7F800000) >> 23) + 15 - 127;
  short res;
  if (exp > 0 && exp < 30) {
    // use rte rounding mode, round the significand, combine sign, exponent and
    // significand into a short.
    res = (sign << 15) | (exp << 10) | ((mantissa + 0x00001000) >> 13);
  } else if (param_bit == 0) {
    res = 0;
  } else {
    if (exp <= 0) {
      if (exp < -10) {
        // value is less than min half float point
        res = 0;
      } else {
        // normalized single, magnitude is less than min normal half float
        // point.
        mantissa = (mantissa | 0x00800000) >> (1 - exp);
        // round to nearest
        if ((mantissa & 0x00001000) > 0) {
          mantissa = mantissa + 0x00002000;
        }
        // combine sign & mantissa (exp is zero to get denormalized number)
        res = (sign << 15) | (mantissa >> 13);
      }
    } else if (exp == (255 - 127 + 15)) {
      if (mantissa == 0) {
        // input float is infinity, return infinity half
        res = (sign << 15) | 0x7C00;
      } else {
        // input float is NaN, return half NaN
        res = (sign << 15) | 0x7C00 | (mantissa >> 13);
      }
    } else {
      // exp > 0, normalized single, round to nearest
      if ((mantissa & 0x00001000) > 0) {
        mantissa = mantissa + 0x00002000;
        if ((mantissa & 0x00800000) > 0) {
          mantissa = 0;
          exp = exp + 1;
        }
      }
      if (exp > 30) {
        // exponent overflow - return infinity half
        res = (sign << 15) | 0x7C00;
      } else {
        // combine sign, exp and mantissa into normalized half
        res = (sign << 15) | (exp << 10) | (mantissa >> 13);
      }
    }
  }
  return res;
}

void weight_pack_vanilla(float* buf, float* data, int G, int dims[4],
                         int type) {
  float* blob = (float*)buf;
  int N = dims[0];
  int C = dims[1];
  int H = dims[2];
  int W = dims[3];

  int channel_per_cube = 8 / 1;
  int w_stride_kgrp = 8 * C * H * W;

  int N_offset = G * N;

  for (int n = 0; n < (N / 8 + 1); n++) {
    int n_size = (N - n * 8 >= 8) ? 8 : N - n * 8;
    int w_stride_surf = W * H * n_size * channel_per_cube;
    for (int h = 0; h < H; h++) {
      for (int w = 0; w < W; w++) {
        for (int n_ofs = 0; n_ofs < n_size; n_ofs++) {
          for (int c = 0; c < C; c++) {
            int surf_ofs = c / channel_per_cube;
            int ch_ofs = c % channel_per_cube;
            int cube_size =
                (C - surf_ofs * channel_per_cube) >= channel_per_cube
                    ? channel_per_cube
                    : (C - surf_ofs * channel_per_cube);
            int w_stride_line = W * n_size * cube_size;

            int blob_ofs = (n * w_stride_kgrp) + (surf_ofs * w_stride_surf) +
                           (h * w_stride_line) + w * n_size * cube_size +
                           (n_ofs * cube_size) + ch_ofs;
            int data_ofs =
                ((n * 8 + n_ofs + N_offset) * C * H * W) + // n = n*16 + n_ofs
                c * H * W +                                // c = c + C_offset
                (h * W) + w;
            *(blob + blob_ofs) =
                (float)__gnu_f2h_ieee_vanilla(*(data + data_ofs));
          }
        }
      }
    }
  }
}

void CHW_to_HWC(float* input_data, int dims[4], float* output_data) {
  unsigned int index = 0;
  for (int k = 0; k < dims[0]; k++) {       // c*h*w
    for (int h = 0; h < dims[2]; h++) {     // w
      for (int w = 0; w < dims[3]; w++) {   // 1
        for (int c = 0; c < dims[1]; c++) { // h*w
          output_data[index] =
              input_data[(k * dims[2] * dims[3] * dims[1]) + w +
                         (c * dims[2] * dims[3]) + (h * dims[3])];
          index++;
        }
      }
    }
  }
}

void CHW_to_HWC_mat(float* input_data, int dims[4], float* output_data) {
  unsigned int index = 0;
  for (int h = 0; h < dims[1]; h++) {       // k*w
    for (int w = 0; w < dims[2]; w++) {     // k
      for (int c = 0; c < dims[0]; c++) {   // k*h*w
        for (int k = 0; k < dims[3]; k++) { // 1
          output_data[index] =
              input_data[(c * dims[2] * dims[1] * dims[3]) +
                         (h * dims[2] * dims[3]) + (w * dims[3]) + k];
          index++;
        }
      }
    }
  }
}

int get_padding(unsigned int input_dim, unsigned int output_dim,
                unsigned int kernel_size, unsigned int stride, float auto_pad) {
  float ans = 0;
  ans = ((((output_dim - 1) * stride) + kernel_size) - input_dim) / 2;
  if (ans - ((int)ans) > 0) {
    ans += auto_pad;
  }
  if (ans < 0) {
    ans = 0;
  }
  return (int)ans;
}

#ifdef __cplusplus
}
#endif
