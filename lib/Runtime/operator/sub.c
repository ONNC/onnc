#include <stdint.h>
typedef int32_t ONNC_INDEX_TYPE;

#include "generic/assign.h"
#include "generic/binary.h"

static float sub_(float a, float b)
{
    return a - b;
}

void ONNC_RUNTIME_sub_float(void* restrict context,
    const float* restrict A, int32_t Adim, const int32_t* restrict Ashape,
    const float* restrict B, int32_t Bdim, const int32_t* restrict Bshape,
    float* restrict C, int32_t Cdim, const int32_t* restrict Cshape)
{
    ONNC_ASSIGN(float, C, Cshape, Cdim, A, Ashape, Adim);
    ONNC_BINARY(float, C, Cshape, Cdim, B, Bshape, Bdim, sub_);
}
