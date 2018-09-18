#include <onnc/Runtime/operator/pad.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CONSTANT_MODE 0
#define EDGE_MODE 1
#define REFLECT_MODE 2

static inline void calculate_axis_dis(int32_t ndim, const int32_t * restrict dims, int32_t * restrict axis_dis){
    int32_t base = axis_dis[ndim-1] = 1;
    for(int32_t dim = ndim - 2 ; dim >= 0 ; --dim){
        axis_dis[dim] = base * dims[dim+1];
        base = axis_dis[dim];
    }
}

static inline bool next_dim(int32_t ndim, int32_t * restrict dim,
                            const int32_t * restrict dim_base, const int32_t * restrict dim_max) {
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
      dim[ndim] = dim_base[ndim];
    }
  } while(true);
}

static inline int64_t dim_to_offset(int32_t ndim, const int32_t * restrict dim,
                                    const int32_t * restrict axisDistance) {
  int64_t offset = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * axisDistance[i];
  }
  return offset;
}

static inline void add_initial_to_output(
    const float * restrict input, int32_t input_ndim, const int32_t * restrict input_dims,
    int32_t * restrict axis_dis, int32_t * pads, int32_t * restrict axis_pad_dis,
    float * restrict output
){
    int32_t iter_index[input_ndim];
    int32_t fill_index[input_ndim];
    memset(iter_index, 0, sizeof(int32_t) * input_ndim);

    int32_t iter_base[input_ndim];
    memset(iter_base, 0, sizeof(int32_t) * input_ndim);
    do{
        memcpy(fill_index, iter_index, sizeof(int32_t) * input_ndim);
        for(int32_t dim = 0 ; dim < input_ndim; dim++){
            fill_index[dim] += pads[dim];
        }

        output[dim_to_offset(input_ndim, fill_index, axis_pad_dis)]
        = input[dim_to_offset(input_ndim, iter_index, axis_dis)];

    }while(next_dim(input_ndim, iter_index, iter_base, input_dims));
}

static inline bool in_obj_area(int32_t * restrict index, int32_t * restrict object, int32_t ndim){
    for(int32_t dim = 0 ; dim < ndim ; dim++){
        if(! (index[dim] >= object[dim] && index[dim] <= object[dim + ndim])){
            return false;
        }
    }
    return true;
}

static inline void pad_along_axis(
    float * restrict output, int32_t output_ndim, const int32_t * restrict output_dims,
    const int32_t * restrict pads, const int32_t * restrict axis_pad_dis, const int32_t axis,
    int32_t * restrict object_area, const int32_t mode
){
    /* iterative walk though dim which axis dim equal to object edge */
    int32_t iter_dim_num = axis;

    /* initial iter_base */
    int32_t iter_base[iter_dim_num];
    memcpy(iter_base, object_area, sizeof(int32_t) * iter_dim_num);

    /* initial iter_dim_max */
    int32_t iter_dim_max[iter_dim_num];
    memcpy(iter_dim_max, &object_area[output_ndim], sizeof(int32_t) * iter_dim_num);
    for(int32_t dim = 0 ; dim < iter_dim_num ; dim++) iter_dim_max[dim] += 1;

    /* initial iter_index */
    int32_t iter_index[iter_dim_num];
    memcpy(iter_index, iter_base, sizeof(int32_t) * iter_dim_num);

    /* initial iter_axis_dis */
    int32_t iter_axis_dis[iter_dim_num];
    memcpy(iter_axis_dis, axis_pad_dis, sizeof(int32_t) * iter_dim_num);

    switch(mode){
        case EDGE_MODE:{
            do{
                int32_t base_offset = dim_to_offset(iter_dim_num, iter_index, iter_axis_dis);

                /* deal with begin pad */
                int32_t clone_start_index = base_offset + object_area[axis] * axis_pad_dis[axis];
                for(int32_t pad = 0; pad < pads[axis] ; pad++){
                    int32_t copy_start_offset = base_offset + pad * axis_pad_dis[axis];
                    memcpy(&output[copy_start_offset], &output[clone_start_index], sizeof(float) * axis_pad_dis[axis]);
                }

                /* deal with end pad */
                int32_t clone_end_index = base_offset + object_area[axis+output_ndim] * axis_pad_dis[axis];
                int32_t end_bound = object_area[axis+output_ndim] + pads[axis + output_ndim];
                for(int32_t pad = object_area[axis+output_ndim] + 1 ; pad <= end_bound; pad++){
                    int32_t copy_end_offset = base_offset + pad * axis_pad_dis[axis];
                    memcpy(&output[copy_end_offset], &output[clone_end_index], sizeof(float) * axis_pad_dis[axis]);
                }

            }while(next_dim(iter_dim_num, iter_index, iter_base, iter_dim_max));
            break;
        }

        case REFLECT_MODE:{
            do{
                int32_t base_offset = dim_to_offset(iter_dim_num, iter_index, iter_axis_dis);

                /* deal with begin pad */
                for(int32_t pad = 0; pad < pads[axis] ; pad++){
                    int32_t copy_start_offset = base_offset + pad * axis_pad_dis[axis];
                    int32_t reflect_dis = object_area[axis] - pad;
                    int32_t clone_start_index = base_offset + (object_area[axis] + reflect_dis) * axis_pad_dis[axis];
                    memcpy(&output[copy_start_offset], &output[clone_start_index], sizeof(float) * axis_pad_dis[axis]);
                }

                /* deal with end pad */

                int32_t end_bound = object_area[axis+output_ndim] + pads[axis + output_ndim];
                for(int32_t pad = object_area[axis+output_ndim] + 1 ; pad <= end_bound; pad++){
                    int32_t copy_end_offset = base_offset + pad * axis_pad_dis[axis];
                    int32_t reflect_dis = pad - object_area[axis+output_ndim];
                    int32_t clone_end_index = base_offset + (object_area[axis+output_ndim] - reflect_dis) * axis_pad_dis[axis];
                    memcpy(&output[copy_end_offset], &output[clone_end_index], sizeof(float) * axis_pad_dis[axis]);
                }
            }while(next_dim(iter_dim_num, iter_index, iter_base, iter_dim_max));
            break;
        }
    }
}

static inline void padding(
    float * restrict output, int32_t output_ndim, const int32_t * restrict output_dims,
    const int32_t * restrict pads, const int32_t * restrict axis_pad_dis,
    int32_t * restrict object_area, const char * restrict mode, float value
){
    int32_t mode_no = 0;
    if(strcmp(mode, "constant") == 0) mode_no = 0;
    else if(strcmp(mode, "edge") == 0) mode_no = 1;
    else if(strcmp(mode, "reflect") == 0) mode_no = 2;

    int32_t iter_index[output_ndim];
    memset(iter_index, 0, sizeof(int32_t) * output_ndim);

    switch(mode_no){
        case CONSTANT_MODE: {
            int32_t iter_base[output_ndim];
            memset(iter_base, 0, sizeof(int32_t) * output_ndim);
            do{
                if(in_obj_area(iter_index, object_area, output_ndim)) continue;
                int32_t offset = dim_to_offset(output_ndim, iter_index, axis_pad_dis);
                output[offset] = value;
            }while(next_dim(output_ndim, iter_index, iter_base, output_dims));
            break;
        }
        case EDGE_MODE: {
            for(int32_t axis = output_ndim - 1; axis >= 0 ; axis--){
                pad_along_axis(output, output_ndim, output_dims, pads, axis_pad_dis, axis, object_area, EDGE_MODE);
            }
            break;
        }
        case REFLECT_MODE:{
            for(int32_t axis = output_ndim - 1; axis >= 0 ; axis--){
                pad_along_axis(output, output_ndim, output_dims, pads, axis_pad_dis, axis, object_area, REFLECT_MODE);
            }
            break;
        }
    }
}

void ONNC_RUNTIME_pad_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_data
  ,int32_t input_data_ndim, const int32_t * restrict input_data_dims
  ,float * restrict output_output
  ,int32_t output_output_ndim, const int32_t * restrict output_output_dims
  ,const char * restrict mode
  ,int32_t * restrict pads
  ,int32_t number_of_pads
  ,float value
) {
    /* calculate axis_dis */
    int32_t axis_dis[input_data_ndim];
    calculate_axis_dis(input_data_ndim, input_data_dims, axis_dis);

    /* calculate input_pad_dims */
    int32_t input_pad_dims[input_data_ndim];
    memcpy(input_pad_dims, input_data_dims, sizeof(int32_t) * input_data_ndim);
    for(int32_t dim = 0 ; dim < input_data_ndim ; dim++){
        input_pad_dims[dim] += (pads[dim] + pads[dim + input_data_ndim]);
    }

    /* calculate axis_pad_dis */
    int32_t axis_pad_dis[input_data_ndim];
    calculate_axis_dis(input_data_ndim, input_pad_dims, axis_pad_dis);

    /* add initial value to output with new index */
    /* TODO: negative padding */
    add_initial_to_output(input_data, input_data_ndim, input_data_dims, axis_dis,
                          pads, axis_pad_dis, output_output);
    /* initial object area */
    int32_t object_area[2*input_data_ndim];
    for(int32_t dim = 0 ; dim < input_data_ndim ; dim++){
        object_area[dim] = pads[dim];
        object_area[dim + input_data_ndim] = input_data_dims[dim] - 1 + pads[dim];
    }
    /* implement padding */
    padding(output_output, output_output_ndim, output_output_dims, pads, axis_pad_dis, object_area, mode, value);
}
