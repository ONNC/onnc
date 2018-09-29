#include <onnc/Runtime/operator/reducemean.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline void calculate_axis_dis(int32_t ndim, const int32_t * restrict dims, int32_t * restrict axis_dis){
  int32_t base = axis_dis[ndim-1] = 1;
  for(int32_t dim = ndim - 2 ; dim >= 0 ; --dim){
    axis_dis[dim] = base * dims[dim+1];
    base = axis_dis[dim];
  }
}

static inline bool next_dim(int32_t ndim, int32_t * restrict dim, const int32_t * restrict dim_max) {
  if(ndim == 0)
    return false;
  do {
    ndim = ndim - 1;
    dim[ndim] += 1;
    if (dim[ndim] < dim_max[ndim]) {
      return true;
    } else { // reach dimension max
      if (ndim == 0) { // all dimension done
        return false;
      }
      dim[ndim] = 0;
    }
  } while(true);
}

static inline int64_t dim_to_offset(int32_t ndim, const int32_t * restrict dim,
                                    const int32_t * restrict axisDistance) {
  if(ndim == 0) return 0;
  int64_t offset = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * axisDistance[i];
  }
  return offset;
}

void ONNC_RUNTIME_reducemean_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_reduced
  ,int32_t output_reduced_ndim, const int32_t * restrict output_reduced_dims
  ,int32_t * restrict axes
  ,int32_t number_of_axes
  ,int32_t keepdims
) {
  /* initial axis */
  int32_t mask = 0;

  /* on bit means in parameter axes */
  for(int32_t axis = 0 ; axis < number_of_axes ; axis++){
      mask |= (1 << axes[axis]);
  }

  /* calculate axis_dis */
  int32_t axis_dis[input_data_ndim];
  calculate_axis_dis(input_data_ndim, input_data_dims, axis_dis);

  /* initial iterative context */
  int32_t not_in_axes_num = input_data_ndim - number_of_axes;
  int32_t iter_not_in_axes_max[not_in_axes_num];
  int32_t not_in_axes_axis_dis[input_data_ndim];
  int32_t not_in_axes_axis_index = 0;

  int32_t iter_in_axes_max[number_of_axes];
  int32_t in_axes_axis_dis[number_of_axes];
  int32_t in_axes_axis_index = 0;
  for(int32_t axis = 0 ; axis < input_data_ndim; axis++){
    if(mask & (1 << axis)){
      in_axes_axis_dis[in_axes_axis_index] = axis_dis[axis];
      iter_in_axes_max[in_axes_axis_index] = input_data_dims[axis];
      in_axes_axis_index += 1;
      continue;
    }
    not_in_axes_axis_dis[not_in_axes_axis_index] = axis_dis[axis];
    iter_not_in_axes_max[not_in_axes_axis_index] = input_data_dims[axis];
    not_in_axes_axis_index += 1;
  }

  /* calculate mean's denominator */
  float mean_denominator = 1.f;
  for(int32_t index = 0 ; index < number_of_axes ; index++) mean_denominator *= iter_in_axes_max[index];

  /* traverse all axis other than parameter axes */
  int32_t insertIndex = 0;
  int32_t iter_not_in_axes[not_in_axes_num];
  memset(iter_not_in_axes, 0, sizeof(int32_t) * not_in_axes_num);
  do{
    int32_t iter_in_axes[number_of_axes];
    memset(iter_in_axes, 0, sizeof(int32_t) * number_of_axes);
    int32_t base_offset = dim_to_offset(not_in_axes_num, iter_not_in_axes, not_in_axes_axis_dis);
    float sum = 0.f;

    /* traver all axes with fixed axis */
    do{
      int32_t offset = base_offset + dim_to_offset(number_of_axes, iter_in_axes, in_axes_axis_dis);
      sum += input_data[offset];
    }while(next_dim(number_of_axes, iter_in_axes, iter_in_axes_max));
    output_reduced[insertIndex++] = sum / mean_denominator;
  }while(next_dim(not_in_axes_num, iter_not_in_axes, iter_not_in_axes_max));
}
