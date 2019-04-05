void ONNC_RUNTIME_reducelogsumexp_float(
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
      sum += expf(input_data[offset]);
    }while(next_dim(number_of_axes, iter_in_axes, iter_in_axes_max));
    output_reduced[insertIndex++] = logf(sum);
  }while(next_dim(not_in_axes_num, iter_not_in_axes, iter_not_in_axes_max));
}
