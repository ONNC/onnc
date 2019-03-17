/*===----- fp16.c - Implement __gnu_f2h_ieee and __gnu_h2f_ieee   ----------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 *===------------------------------------------------------------------------------===
 *
 * This file implements __gnu_f2h_ieee and __gnu_h2f_ieee for the compiler_rt library.
 *
 *===------------------------------------------------------------------------------===
 * champ - get from https://reviews.llvm.org/D4927
 */

// ------------------------------

#include "NvDlaDefine.h"

// ------------------------------

#ifdef __cplusplus
extern "C" {
#endif

float __gnu_h2f_ieee(short param)
{
  unsigned short expHalf16  = param & 0x7C00;
  int            exp1       = (int)expHalf16;
  unsigned short mantissa16 = param & 0x03FF;
  int            mantissa1  = (int)mantissa16;
  int            sign       = (int)(param & 0x8000);
  sign                      = sign << 16;

  // nan or inf
  if (expHalf16 == 0x7C00) {
    // nan
    if (mantissa16 > 0) {
      int   res  = (0x7FC00000 | sign);
      float fres = *((float*)(&res));
      return fres;
    }
    // inf
    int   res  = (0x7F800000 | sign);
    float fres = *((float*)(&res));
    return fres;
  }
  if (expHalf16 != 0) {
    exp1 += ((127 - 15) << 10); // exponents converted to float32 bias
    int res    = (exp1 | mantissa1);
    res        = res << 13;
    res        = (res | sign);
    float fres = *((float*)(&res));
    return fres;
  }

  int xmm1 = exp1 > (1 << 10) ? exp1 : (1 << 10);
  xmm1     = (xmm1 << 13);
  xmm1 += ((127 - 15 - 10) << 23); // add the bias difference to xmm1
  xmm1 = xmm1 | sign;              // Combine with the sign mask

  float res = (float)mantissa1; // Convert mantissa to float
  res *= *((float*)(&xmm1));

  return res;
}

short __gnu_f2h_ieee(float param)
{
  unsigned int param_bit = *((unsigned int*)(&param));
  int          sign      = param_bit >> 31;
  int          mantissa  = param_bit & 0x007FFFFF;
  int          exp       = ((param_bit & 0x7F800000) >> 23) + 15 - 127;
  short        res;
  if (exp > 0 && exp < 30) {
    // use rte rounding mode, round the significand, combine sign, exponent and significand into a short.
    res = (sign << 15) | (exp << 10) | ((mantissa + 0x00001000) >> 13);
  } else if (param_bit == 0) {
    res = 0;
  } else {
    if (exp <= 0) {
      if (exp < -10) {
        // value is less than min half float point
        res = 0;
      } else {
        // normalized single, magnitude is less than min normal half float point.
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
          exp      = exp + 1;
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

void weight_pack(void* buf, float* data, unsigned long long size, int G, int dims[4], int type, _Bool shouldPadZero)
{
  nv_weight_t* blob = (nv_weight_t*)buf;
  int          N    = dims[0];
  int          C    = dims[1];
  int          H    = dims[2];
  int          W    = dims[3];

  int channel_per_cube = WEIGHT_ATOM_CUBE_SIZE / ELEMENT_SIZE;
  int w_stride_kgrp    = MAC_ATOMIC_K * C * H * W;
#if 0
  int C_offset = G*C;

  for(int n = 0; n < (N/16 + 1); n++){
    int n_size = (N - n*16 >= 16) ? 16 : N - n*16;
    int w_stride_surf = W * H * n_size * channel_per_cube;
    for(int h = 0; h < H; h++){
      for(int w = 0; w < W; w++){
        for(int n_ofs = 0; n_ofs < n_size; n_ofs++){
          for(int c = 0; c < C; c++){
            int surf_ofs = c / channel_per_cube;
            int ch_ofs = c % channel_per_cube;
            int cube_size = (C - surf_ofs*channel_per_cube) >= channel_per_cube ? channel_per_cube : (C - surf_ofs*channel_per_cube);
            int w_stride_line = W * n_size * cube_size;

            int blob_ofs =  (n * w_stride_kgrp) +
                            (surf_ofs * w_stride_surf) +
                            (h * w_stride_line) +
                            w * n_size * cube_size +
                            (n_ofs * cube_size) + ch_ofs;
            int data_ofs =  ((n*16 + n_ofs) * (G*C) * H * W) +    // n = n*16 + n_ofs
                            ((c + C_offset)) * H * W +            // c = c + C_offset
                            (h * W) + w;
#  if HAS_IMAGE_MODE
            if (shouldPadZero) {
              // FIXME: Assume the given image has 3 channels only, not 4 channels. So the 4th channel weights are 0.
              if (C == 4 && c == 3) {
                *(blob + blob_ofs) = 0;
                continue;
              }
            }
#  endif
            *(blob + blob_ofs) = __gnu_f2h_ieee(*(data + data_ofs));
          }
        }
      }
    }
  }
#else
  int N_offset = G * N;
  for (int n = 0; n < (N / MAC_ATOMIC_K + 1); n++) {
    int n_size        = (N - n * MAC_ATOMIC_K >= MAC_ATOMIC_K) ? MAC_ATOMIC_K : N - n * MAC_ATOMIC_K;
    int w_stride_surf = W * H * n_size * channel_per_cube;
    for (int h = 0; h < H; h++) {
      for (int w = 0; w < W; w++) {
        for (int n_ofs = 0; n_ofs < n_size; n_ofs++) {
          for (int c = 0; c < C; c++) {
            int surf_ofs  = c / channel_per_cube;
            int ch_ofs    = c % channel_per_cube;
            int cube_size = (C - surf_ofs * channel_per_cube) >= channel_per_cube ? channel_per_cube
                                                                                  : (C - surf_ofs * channel_per_cube);
            int w_stride_line = W * n_size * cube_size;

            int blob_ofs = (n * w_stride_kgrp) + (surf_ofs * w_stride_surf) + (h * w_stride_line) +
                           w * n_size * cube_size + (n_ofs * cube_size) + ch_ofs;
            int data_ofs = ((n * MAC_ATOMIC_K + n_ofs + N_offset) * C * H * W) + // n = n*16 + n_ofs
                           c * H * W +                                           // c = c + C_offset
                           (h * W) + w;
#  if HAS_IMAGE_MODE
            if (shouldPadZero) {
              // FIXME: Assume the given image has 3 channels only, not 4 channels. So the 4th channel weights are 0.
              if (C == 4 && c == 3) {
                *(blob + blob_ofs) = 0;
                continue;
              }
            }
#  endif
            if (size <= data_ofs) {
              *(blob + blob_ofs) = 0;
              continue;
            }

            *(blob + blob_ofs) = (nv_weight_t)__gnu_f2h_ieee(
              *(data + data_ofs)); // FIXME: hack for solving memory allocation issue. Probably we should introduce
                                   // __gnu_f2c(har)_ieee or something like that.
          }
        }
      }
    }
  }
#endif
}

#ifdef __cplusplus
}
#endif
