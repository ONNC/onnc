#include <onnc/Runtime/operator/spacetodepth.h>

#include <stdint.h>
#include <stdbool.h>

static void calculateAxisDistance(int32_t ndim, const int32_t * restrict dims, int32_t * restrict axis_dis){
  int32_t base = axis_dis[ndim - 1] = 1;
  for(int32_t dim = ndim-2 ; dim >= 0 ; dim--){
    axis_dis[dim] = base * dims[dim+1];
    base = axis_dis[dim];
  }
}

static int32_t dim_to_offset(int32_t N, int32_t C, int32_t H, int32_t W, int32_t * restrict axis_dis){
  return N * axis_dis[0] + C * axis_dis[1] + H * axis_dis[2] + W * axis_dis[3];
}

#include "internal/spacetodepth.inc"