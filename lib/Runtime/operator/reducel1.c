#include <onnc/Runtime/operator/reducel1.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static inline void calculate_axis_dis(int32_t ndim, const int32_t* restrict dims, int32_t* restrict axis_dis)
{
  int32_t base = axis_dis[ndim - 1] = 1;
  for (int32_t dim = ndim - 2; dim >= 0; --dim) {
    axis_dis[dim] = base * dims[dim + 1];
    base          = axis_dis[dim];
  }
}

static inline bool next_dim(int32_t ndim, int32_t* restrict dim, const int32_t* restrict dim_max)
{
  if (ndim == 0)
    return false;
  do {
    ndim = ndim - 1;
    dim[ndim] += 1;
    if (dim[ndim] < dim_max[ndim]) {
      return true;
    } else {           // reach dimension max
      if (ndim == 0) { // all dimension done
        return false;
      }
      dim[ndim] = 0;
    }
  } while (true);
}

static inline int64_t dim_to_offset(int32_t ndim, const int32_t* restrict dim, const int32_t* restrict axisDistance)
{
  if (ndim == 0)
    return 0;
  int64_t offset = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * axisDistance[i];
  }
  return offset;
}

#include "internal/reducel1.inc"