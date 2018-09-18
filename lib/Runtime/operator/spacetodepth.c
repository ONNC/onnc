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

void ONNC_RUNTIME_spacetodepth_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_input
  ,int32_t input_input_ndim, const int32_t * restrict input_input_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,int32_t blocksize
) {
    /* calculate axis distance */
    int32_t axis_dis[input_input_ndim];
    calculateAxisDistance(input_input_ndim, input_input_dims, axis_dis);
    int32_t insertIndex = 0;
    int32_t H_num = input_input_dims[2] / blocksize;
    int32_t W_num = input_input_dims[3] / blocksize;
    for(int32_t N = 0 ; N < input_input_dims[0] ; ++N){
      for(int32_t C = 0 ; C < input_input_dims[1] ; ++C){
        for(int32_t block_H = 0 ; block_H < H_num ; ++block_H){
          for(int32_t block_W = 0 ; block_W < W_num ; ++block_W){
            for(int32_t fill_H = block_H * blocksize ; fill_H < (block_H + 1) * blocksize ; ++fill_H){
              for(int32_t fill_W = block_W * blocksize; fill_W < (block_W + 1) * blocksize ; ++fill_W){
                output_output[insertIndex++] = input_input[dim_to_offset(N, C, fill_H, fill_W, axis_dis)];
              }
            }
          }
        }
      }
    }
}
