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
