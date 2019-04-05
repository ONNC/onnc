void ONNC_RUNTIME_topk_float(
  void * restrict onnc_runtime_context
  ,const float * restrict input_X
  ,int32_t input_X_ndim, const int32_t * restrict input_X_dims
  ,float * restrict output_Values
  ,int32_t output_Values_ndim, const int32_t * restrict output_Values_dims
  ,float * restrict output_Indices
  ,int32_t output_Indices_ndim, const int32_t * restrict output_Indices_dims
  ,int32_t axis
  ,int32_t k
) {
  int32_t axis_dis[input_X_ndim];
  memset(axis_dis, 0, sizeof(int32_t) * input_X_ndim);

  /* calculate_axis_dis */
  calculate_axis_dis(input_X_ndim, input_X_dims, axis_dis);

  /* initialize iter context */
  int32_t iter_ndim = input_X_ndim - 1;
  int32_t iter_max[iter_ndim];
  int32_t iter_axis_dis[iter_ndim];
  int32_t iter_index = 0;
  for(int32_t axis_index = 0 ; axis_index < input_X_ndim ; ++axis_index){
      if(axis_index == axis) continue;
      iter_max[iter_index] = input_X_dims[axis_index];
      iter_axis_dis[iter_index] = axis_dis[axis_index];
      iter_index += 1;
  }
  int32_t iter_base[iter_ndim];
  memset(iter_base, 0, sizeof(int32_t) * iter_ndim);

  int32_t insertIndex = 0;

  /* traverse */
  do{
      int32_t base_offset = dim_to_offset(iter_ndim, iter_base, iter_axis_dis);
      struct node heap[k];
      memset(heap, 0, sizeof(struct node) * k);
      int32_t inside_num = 0;
      for(int32_t axis_index = 0 ; axis_index < input_X_dims[axis]; ++axis_index){
          int32_t offset = base_offset + axis_dis[axis] * axis_index;
          int32_t value = input_X[offset];
          if(inside_num < k){
              insert_node(heap, inside_num, value, axis_index);
              inside_num ++;
          }
          else{
              if(value <= heap[1].value) continue;
              init(heap, 1, value, axis_index);
              down_heap(heap, 1, k);
          }
      }
      struct node output_temp[k];
      memset(output_temp, 0, sizeof(struct node) * k);
      for(int32_t index = k - 1; index >= 0 ; --index){
          output_temp[index] = heap[1];
          delete_node(heap, index + 1);
      }

      for(int32_t index = 0 ; index < k ; ++index){
        output_Values[insertIndex] = output_temp[index].value;
        output_Indices[insertIndex] = output_temp[index].index;
        insertIndex++;
      }
  }while(next_dim(iter_ndim, iter_base, iter_max));
}
