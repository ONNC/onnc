int32_t spacetodepth_dim_to_offset(int32_t N, int32_t C, int32_t H, int32_t W, int32_t* restrict axis_dis)
{
  return N * axis_dis[0] + C * axis_dis[1] + H * axis_dis[2] + W * axis_dis[3];
}

#include "spacetodepth.impl.inc"
