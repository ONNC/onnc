#ifndef ONNC_RUNTIME_INTERNAL_COMMON_H_INCLUDED
#define ONNC_RUNTIME_INTERNAL_COMMON_H_INCLUDED

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int64_t getTensorSize(size_t numOfDimensions, const int32_t* dimensions);
int32_t findIndex(int32_t index, int32_t axisDim, int32_t elementDistance);
void    calculate_axis_dis(int32_t ndim, const int32_t* restrict dims, int32_t* restrict axis_dis);
bool    next_dim_no_check(int32_t ndim, int32_t* restrict dim, const int32_t* restrict dim_max);
bool    next_dim(int32_t ndim, int32_t* restrict dim, const int32_t* restrict dim_max);
int64_t dim_to_offset(int32_t ndim, const int32_t* restrict dim, const int32_t* restrict axisDistance);
int64_t dim_to_offset_with_step(int32_t ndim, const int32_t* restrict dim, const int32_t* restrict dim_max);
float   get_value_or_zero(int32_t ndim, const int32_t* restrict dim_max, const float* restrict value,
                          const int32_t* restrict dim);
float   get_value_or_zero_5(int32_t ndim, const int32_t* restrict dim_max, const float* restrict value,
                            const int32_t* restrict dim, int32_t* isPad);
int32_t con(int32_t* meofarr, int32_t ndim, const int32_t* dims);
float   randomNormal(float mean, float stddev);

#endif
