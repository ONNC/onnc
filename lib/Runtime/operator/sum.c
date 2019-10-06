#include <stdint.h>
typedef int32_t ONNC_INDEX_TYPE;

#include "generic/assign.h"
#include "generic/binary.h"
#include "generic/size.h"
#include <string.h>

static float sum_(float a, float b)
{
    return a + b;
}

void ONNC_RUNTIME_sum_float(void* restrict context,
    const float* restrict* inputs, int32_t count, const int32_t* restrict ndims, const int32_t* restrict* shapes,
    float* restrict output, int32_t ndim, const int32_t* restrict shape)
{
    if (count) {
        ONNC_ASSIGN(float, output, shape, ndim, inputs[0], shapes[0], ndims[0]);

        for (int32_t i = 1; i < count; ++i)
            ONNC_BINARY(float, output, shape, ndim, inputs[i], shapes[i], ndims[i], sum_);
    }
    else {
        memset(output, 0, onnc_size(shape, ndim) * sizeof(float));
    }
}
