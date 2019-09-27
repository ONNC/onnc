#include "WeightPack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

void CHW_to_HWC(float* input_data, int dims[4], float* output_data)
{
  unsigned int index = 0;
  for (int k = 0; k < dims[0]; k++) {       // c*h*w
    for (int h = 0; h < dims[2]; h++) {     // w
      for (int w = 0; w < dims[3]; w++) {   // 1
        for (int c = 0; c < dims[1]; c++) { // h*w
          output_data[index] =
            input_data[(k * dims[2] * dims[3] * dims[1]) + w + (c * dims[2] * dims[3]) + (h * dims[3])];
          index++;
        }
      }
    }
  }
}

void CHW_to_HWC_mat(float* input_data, int dims[4], float* output_data)
{
  unsigned int index = 0;
  for (int h = 0; h < dims[1]; h++) {       // k*w
    for (int w = 0; w < dims[2]; w++) {     // k
      for (int c = 0; c < dims[0]; c++) {   // k*h*w
        for (int k = 0; k < dims[3]; k++) { // 1
          output_data[index] =
            input_data[(c * dims[2] * dims[1] * dims[3]) + (h * dims[2] * dims[3]) + (w * dims[3]) + k];
          index++;
        }
      }
    }
  }
}

int get_padding(unsigned int input_dim, unsigned int output_dim, unsigned int kernel_size, unsigned int stride,
                float auto_pad)
{
  float ans = 0;
  ans       = ((((output_dim - 1) * stride) + kernel_size) - input_dim) / 2;
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
