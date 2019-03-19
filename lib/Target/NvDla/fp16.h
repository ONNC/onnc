#ifndef _FP16_H_
#define _FP16_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

float __gnu_h2f_ieee(short param);
short __gnu_f2h_ieee(float param);
void  weight_pack(void* buf, const float* data, unsigned long long size, int G, int dims[4], int type,
                  _Bool shouldPadZero);

#ifdef __cplusplus
}
#endif

#endif
